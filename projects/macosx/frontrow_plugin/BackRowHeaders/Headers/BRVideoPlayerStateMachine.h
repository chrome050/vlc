/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSObject.h"

@class BRVideoPlayerState;

@interface BRVideoPlayerStateMachine : NSObject
{
    BRVideoPlayerState *_currentState;
}

- (id)init;
- (void)dealloc;
- (void)reset;
- (id)currentState;
- (int)updateStateWithAction:(int)fp8;

@end

