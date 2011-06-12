/*****************************************************************************
 * VideoEffects.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2011 Felix Paul Kühne
 * $Id$
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
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

#import <Cocoa/Cocoa.h>


@interface VLCVideoEffects : NSObject {
    /* generic */
    intf_thread_t *p_intf;
    IBOutlet id o_window;
    IBOutlet id o_tableView;

    /* basic */
    IBOutlet id o_adjust_ckb;
    IBOutlet id o_adjust_hue_lbl;
    IBOutlet id o_adjust_hue_sld;
    IBOutlet id o_adjust_contrast_lbl;
    IBOutlet id o_adjust_contrast_sld;
    IBOutlet id o_adjust_brightness_lbl;
    IBOutlet id o_adjust_brightness_sld;
    IBOutlet id o_adjust_brightness_ckb;
    IBOutlet id o_adjust_saturation_lbl;
    IBOutlet id o_adjust_saturation_sld;
    IBOutlet id o_adjust_gamma_lbl;
    IBOutlet id o_adjust_gamma_sld;
    IBOutlet id o_adjust_opaque_lbl;
    IBOutlet id o_adjust_opaque_sld;
    IBOutlet id o_sharpen_ckb;
    IBOutlet id o_sharpen_lbl;
    IBOutlet id o_sharpen_sld;
    IBOutlet id o_banding_ckb;
    IBOutlet id o_banding_lbl;
    IBOutlet id o_banding_sld;
    IBOutlet id o_grain_ckb;
    IBOutlet id o_grain_sld;
    IBOutlet id o_grain_lbl;

    /* crop */
    IBOutlet id o_crop_top_lbl;
    IBOutlet id o_crop_top_fld;
    IBOutlet id o_crop_left_lbl;
    IBOutlet id o_crop_left_fld;
    IBOutlet id o_crop_right_lbl;
    IBOutlet id o_crop_right_fld;
    IBOutlet id o_crop_bottom_lbl;
    IBOutlet id o_crop_bottom_fld;
    IBOutlet id o_crop_sync_top_bottom_ckb;
    IBOutlet id o_crop_sync_left_right_ckb;

    /* geometry */
    IBOutlet id o_transform_ckb;
    IBOutlet id o_transform_pop;
    IBOutlet id o_zoom_ckb;
    IBOutlet id o_puzzle_ckb;
    IBOutlet id o_puzzle_rows_lbl;
    IBOutlet id o_puzzle_rows_fld;
    IBOutlet id o_puzzle_columns_lbl;
    IBOutlet id o_puzzle_columns_fld;
    IBOutlet id o_puzzle_blackslot_ckb;

    /* color */
    IBOutlet id o_threshold_ckb;
    IBOutlet id o_threshold_color_lbl;
    IBOutlet id o_threshold_color_fld;
    IBOutlet id o_threshold_saturation_lbl;
    IBOutlet id o_threshold_saturation_sld;
    IBOutlet id o_threshold_similarity_lbl;
    IBOutlet id o_threshold_similarity_sld;
    IBOutlet id o_sepia_ckb;
    IBOutlet id o_sepia_lbl;
    IBOutlet id o_sepia_fld;
    IBOutlet id o_noise_ckb;
    IBOutlet id o_gradient_ckb;
    IBOutlet id o_gradient_mode_lbl;
    IBOutlet id o_gradient_mode_pop;
    IBOutlet id o_gradient_color_ckb;
    IBOutlet id o_gradient_cartoon_ckb;
    IBOutlet id o_extract_ckb;
    IBOutlet id o_extract_lbl;
    IBOutlet id o_extract_fld;
    IBOutlet id o_invert_ckb;
    IBOutlet id o_posterize_ckb;
    IBOutlet id o_posterize_lbl;
    IBOutlet id o_posterize_fld;
    IBOutlet id o_blur_ckb;
    IBOutlet id o_blur_sld;
    IBOutlet id o_blur_lbl;
    IBOutlet id o_motiondetect_ckb;
    IBOutlet id o_watereffect_ckb;
    IBOutlet id o_waves_ckb;
    IBOutlet id o_psychedelic_ckb;

    /* video output & overlay */
    IBOutlet id o_clone_ckb;
    IBOutlet id o_clone_lbl;
    IBOutlet id o_clone_fld;
    IBOutlet id o_addtext_ckb;
    IBOutlet id o_addtext_text_fld;
    IBOutlet id o_addtext_text_lbl;
    IBOutlet id o_addtext_pos_lbl;
    IBOutlet id o_addtext_pos_pop;

    /* Logo */
    IBOutlet id o_addlogo_ckb;
    IBOutlet id o_addlogo_logo_lbl;
    IBOutlet id o_addlogo_logo_fld;
    IBOutlet id o_addlogo_top_lbl;
    IBOutlet id o_addlogo_top_fld;
    IBOutlet id o_addlogo_left_lbl;
    IBOutlet id o_addlogo_left_fld;
    IBOutlet id o_addlogo_transparency_lbl;
    IBOutlet id o_addlogo_transparency_sld;
    IBOutlet id o_eraselogo_ckb;
    IBOutlet id o_eraselogo_mask_lbl;
    IBOutlet id o_eraselogo_mask_fld;
    IBOutlet id o_eraselogo_top_lbl;
    IBOutlet id o_eraselogo_top_fld;
    IBOutlet id o_eraselogo_left_lbl;
    IBOutlet id o_eraselogo_left_fld;
}

/* generic */
+ (VLCVideoEffects *)sharedInstance;
- (IBAction)toggleWindow:(id)sender;
- (void)resetValues;
- (void)setVideoFilter: (char *)psz_name on:(BOOL)b_on;

/* basic */
- (IBAction)enableAdjust:(id)sender;
- (IBAction)adjustSliderChanged:(id)sender;
- (IBAction)enableAdjustBrightnessThreshold:(id)sender;
- (IBAction)enableSharpen:(id)sender;
- (IBAction)sharpenSliderChanged:(id)sender;
- (IBAction)enableBanding:(id)sender;
- (IBAction)bandingSliderChanged:(id)sender;
- (IBAction)enableGrain:(id)sender;
- (IBAction)grainSliderChanged:(id)sender;

/* crop */
- (IBAction)cropFieldChanged:(id)sender;
- (IBAction)enableCropModifier:(id)sender;

/* geometry */
- (IBAction)enableTransform:(id)sender;
- (IBAction)transformModifierChanged:(id)sender;
- (IBAction)enableZoom:(id)sender;
- (IBAction)enablePuzzle:(id)sender;
- (IBAction)puzzleModifierChanged:(id)sender;

/* color */
- (IBAction)enableThreshold:(id)sender;
- (IBAction)thresholdModifierChanged:(id)sender;
- (IBAction)enableSepia:(id)sender;
- (IBAction)sepiaModifierChanged:(id)sender;
- (IBAction)enableNoise:(id)sender;
- (IBAction)enableGradient:(id)sender;
- (IBAction)gradientModifierChanged:(id)sender;
- (IBAction)enableExtract:(id)sender;
- (IBAction)extractModifierChanged:(id)sender;
- (IBAction)enableInvert:(id)sender;
- (IBAction)enablePosterize:(id)sender;
- (IBAction)posterizeModifierChanged:(id)sender;
- (IBAction)enableBlur:(id)sender;
- (IBAction)blurModifierChanged:(id)sender;
- (IBAction)enableMotionDetect:(id)sender;
- (IBAction)enableWaterEffect:(id)sender;
- (IBAction)enableWaves:(id)sender;
- (IBAction)enablePsychedelic:(id)sender;

/* video output & overlay */
- (IBAction)enableClone:(id)sender;
- (IBAction)cloneModifierChanged:(id)sender;
- (IBAction)enableAddText:(id)sender;
- (IBAction)addTextModifierChanged:(id)sender;

/* logo */
- (IBAction)enableAddLogo:(id)sender;
- (IBAction)addLogoModifierChanged:(id)sender;
- (IBAction)enableEraseLogo:(id)sender;
- (IBAction)eraseLogoModifierChanged:(id)sender;
@end