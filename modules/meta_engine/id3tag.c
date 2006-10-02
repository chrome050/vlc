/*****************************************************************************
 * id3tag.c: id3 tag parser/skipper based on libid3tag
 *****************************************************************************
 * Copyright (C) 2002-2004 the VideoLAN team
 * $Id$
 *
 * Authors: Sigmund Augdal Helberg <dnumgis@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */
#include <string.h>

#include <vlc/vlc.h>
#include <vlc/intf.h>
#include <vlc/input.h>

#include <sys/types.h>

#include "vlc_meta.h"

#include <id3tag.h>
#include "id3genres.h"

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  ParseID3Tags ( vlc_object_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin();
    set_description( _("ID3 tags parser" ) );
    set_capability( "meta reader", 70 );
    set_callbacks( ParseID3Tags, NULL );
vlc_module_end();

/*****************************************************************************
 * Definitions of structures  and functions used by this plugins
 *****************************************************************************/

/*****************************************************************************
 * ParseID3Tag : parse an id3tag into the info structures
 *****************************************************************************/
static void ParseID3Tag( demux_t *p_demux, uint8_t *p_data, int i_size )
{
    struct id3_tag   *p_id3_tag;
    struct id3_frame *p_frame;
    int i = 0;

    p_id3_tag = id3_tag_parse( p_data, i_size );
    if( !p_id3_tag ) return;

    if( !p_demux->p_private ) p_demux->p_private = (void *)vlc_meta_New();

    vlc_meta_t *p_meta = (vlc_meta_t *)(p_demux->p_private);
#define ID_IS( a ) (!strcmp(  p_frame->id, a ))
#define DESCR_IS( a) strstr( (char*)p_frame->description, a )

    while ( ( p_frame = id3_tag_findframe( p_id3_tag, "UFID", i ) ) )
    {
        char *psz_owner = id3_field_getlatin1( &p_frame->fields[0] );

        if( !strncmp( psz_owner, "http://musicbrainz.org", 22 ) )
        {
            id3_byte_t const * p_ufid;
            id3_length_t i_ufidlen;

            p_ufid = id3_field_getbinarydata( &p_frame->fields[1], &i_ufidlen );
            char *psz_ufid = strndup( p_ufid, i_ufidlen );

            vlc_meta_SetTrackID( p_meta, psz_ufid );
            free( psz_ufid );
        }
        i++;
    }

    i = 0;

    while( ( p_frame = id3_tag_findframe( p_id3_tag, "TXXX", i ) ) )
    {
        char *psz_desc = id3_ucs4_latin1duplicate(
                id3_field_getstring( &p_frame->fields[1] ) );

        if ( ! strncmp( psz_desc, "MusicBrainz Artist Id", 21 ) )
        {
            char *psz_artistid = id3_ucs4_latin1duplicate(
                    id3_field_getstring( &p_frame->fields[2] ) );
            vlc_meta_SetArtistID( p_meta, psz_artistid );
            free( psz_artistid );
        }

        if ( ! strncmp( psz_desc, "MusicBrainz Album Id", 20 ) )
        {
            char *psz_albumid = id3_ucs4_latin1duplicate(
                    id3_field_getstring( &p_frame->fields[2] ) );
            vlc_meta_SetAlbumID( p_meta, psz_albumid );
            free( psz_albumid );
        }

        free( psz_desc );
        i++;
    }
    
    i = 0;

    while( ( p_frame = id3_tag_findframe( p_id3_tag , "T", i ) ) )
    {
        int i_strings = id3_field_getnstrings( &p_frame->fields[1] );

        while( i_strings > 0 )
        {
            char *psz_temp = id3_ucs4_utf8duplicate(
                id3_field_getstrings( &p_frame->fields[1], --i_strings ) );

            if( ID_IS( ID3_FRAME_GENRE ) )
            {
                char *psz_endptr;
                int i_genre = strtol( psz_temp, &psz_endptr, 10 );

                if( psz_temp != psz_endptr &&
                    i_genre >= 0 && i_genre < NUM_GENRES )
                {
                    vlc_meta_SetGenre( p_meta, ppsz_genres[atoi(psz_temp)]);
                }
                else
                {
                    /* Unknown genre */
                    vlc_meta_SetGenre( p_meta,psz_temp );
                }
            }
            else if( ID_IS( ID3_FRAME_TITLE ) )
            {
                vlc_meta_SetTitle( p_meta, psz_temp );
            }
            else if( ID_IS( ID3_FRAME_ARTIST ) )
            {
                vlc_meta_SetArtist( p_meta, psz_temp );
            }
            else if( ID_IS( ID3_FRAME_YEAR ) )
            {
                vlc_meta_SetDate( p_meta, psz_temp );
            }
            else if( ID_IS( ID3_FRAME_COMMENT ) )
            {
                vlc_meta_SetDescription( p_meta, psz_temp );
            }
            else if( DESCR_IS( "Copyright" ) )
            {
                vlc_meta_SetCopyright( p_meta, psz_temp );
            }
            else if( DESCR_IS( "Publisher" ) )
            {
                vlc_meta_SetPublisher( p_meta, psz_temp );
            }
            else if( DESCR_IS( "Track number/position in set" ) )
            {
                vlc_meta_SetTracknum( p_meta, psz_temp );
            }
            else if( DESCR_IS( "Album/movie/show title" ) )
            {
                vlc_meta_SetAlbum( p_meta, psz_temp );
            }
            else if( DESCR_IS( "Encoded by" ) )
            {
                vlc_meta_SetEncodedBy( p_meta, psz_temp );
            }
            else if( ID_IS ( "APIC" ) )
            {
                msg_Dbg( p_demux, "** Has APIC **" );
            }
            else if( p_frame->description )
            { /* Unhandled meta*/
                msg_Warn( p_demux, "Fixme: unhandled ID3 metatag, %s", p_frame->description );
            }
            free( psz_temp );
        }
        i++;
    }
    id3_tag_delete( p_id3_tag );
}

/*****************************************************************************
 * ParseID3Tags: check if ID3 tags at common locations. Parse them and skip it
 * if it's at the start of the file
 ****************************************************************************/
static int ParseID3Tags( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t *)p_this;
    uint8_t *p_peek;
    vlc_bool_t b_seekable;
    int64_t i_init, i_pos;
    int i_size;

    p_demux->p_private = NULL;

    msg_Dbg( p_demux, "checking for ID3 tag" );

    stream_Control( p_demux->s, STREAM_CAN_FASTSEEK, &b_seekable );
    if( !b_seekable ) return VLC_SUCCESS;

    i_init = stream_Tell( p_demux->s );

    /*
     * Look for a ID3v1 tag at the end of the file
     */
    i_init = stream_Tell( p_demux->s );
    i_pos = stream_Size( p_demux->s );

    while( i_pos > 128 ) /* while used so we can break; */
    {
        stream_Seek( p_demux->s, i_pos - 128 );

        /* get 10 byte id3 header */
        if( stream_Peek( p_demux->s, &p_peek, 10 ) < 10 ) break;

        i_size = id3_tag_query( p_peek, 10 );
        if( i_size == 128 )
        {
            /* peek the entire tag */
            if( stream_Peek( p_demux->s, &p_peek, i_size ) < i_size ) break;

            msg_Dbg( p_demux, "found ID3v1 tag" );
            ParseID3Tag( p_demux, p_peek, i_size );
        }

        /* look for ID3v2.4 tag at end of file */
        /* get 10 byte ID3 footer */
        if( stream_Peek( p_demux->s, &p_peek, 128 ) < 128 ) break;

        i_size = id3_tag_query( p_peek + 118, 10 );
        if( i_size < 0  && i_pos > -i_size )
        {
            /* id3v2.4 footer found */
            stream_Seek( p_demux->s , i_pos + i_size );
            /* peek the entire tag */
            if( stream_Peek( p_demux->s, &p_peek, i_size ) < i_size ) break;

            msg_Dbg( p_demux, "found ID3v2 tag at end of file" );
            ParseID3Tag( p_demux, p_peek, i_size );
        }
        break;
    }

    /*
     * Get 10 byte id3 header
     */
    stream_Seek( p_demux->s, 0 );
    if( stream_Peek( p_demux->s, &p_peek, 10 ) < 10 ) goto end;

    if( (i_size = id3_tag_query( p_peek, 10 )) <= 0 ) goto end;

    if( stream_Peek( p_demux->s, &p_peek, i_size ) < i_size ) goto end;

    msg_Dbg( p_demux, "found ID3v2 tag" );
    ParseID3Tag( p_demux, p_peek, i_size );

 end:
    stream_Seek( p_demux->s, i_init );
    return VLC_SUCCESS;
}
