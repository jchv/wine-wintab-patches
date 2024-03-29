/*
 * Copyright 2018 Józef Kucia
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef INTERACTION_CONTEXT_H
#define INTERACTION_CONTEXT_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

DECLARE_HANDLE(HINTERACTIONCONTEXT);

typedef enum INTERACTION_CONTEXT_PROPERTY
{
    INTERACTION_CONTEXT_PROPERTY_MEASUREMENT_UNITS = 1,
    INTERACTION_CONTEXT_PROPERTY_INTERACTION_UI_FEEDBACK = 2,
    INTERACTION_CONTEXT_PROPERTY_FILTER_POINTERS = 3,
    INTERACTION_CONTEXT_PROPERTY_MAX = 0xffffffff,
} INTERACTION_CONTEXT_PROPERTY;

typedef enum INTERACTION_ID
{
    INTERACTION_ID_NONE = 0,
    INTERACTION_ID_MANIPULATION = 1,
    INTERACTION_ID_TAP = 2,
    INTERACTION_ID_SECONDARY_TAP = 3,
    INTERACTION_ID_HOLD = 4,
    INTERACTION_ID_DRAG = 5,
    INTERACTION_ID_CROSS_SLIDE = 6,
    INTERACTION_ID_MAX = 0xffffffff,
} INTERACTION_ID;

typedef enum INTERACTION_FLAGS
{
    INTERACTION_FLAG_NONE    = 0x00000000,
    INTERACTION_FLAG_BEGIN   = 0x00000001,
    INTERACTION_FLAG_END     = 0x00000002,
    INTERACTION_FLAG_CANCEL  = 0x00000004,
    INTERACTION_FLAG_INERTIA = 0x00000008,
    INTERACTION_FLAG_MAX     = 0xffffffff,
} INTERACTION_FLAGS;

typedef enum INTERACTION_CONFIGURATION_FLAGS
{
    INTERACTION_CONFIGURATION_FLAG_NONE                             = 0x00000000,

    INTERACTION_CONFIGURATION_FLAG_MANIPULATION                     = 0x00000001,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_TRANSLATION_X       = 0x00000002,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_TRANSLATION_Y       = 0x00000004,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_ROTATION            = 0x00000008,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_SCALING             = 0x00000010,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_TRANSLATION_INERTIA = 0x00000020,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_ROTATION_INERTIA    = 0x00000040,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_SCALING_INERTIA     = 0x00000080,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_RAILS_X             = 0x00000100,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_RAILS_Y             = 0x00000200,
    INTERACTION_CONFIGURATION_FLAG_MANIPULATION_EXACT               = 0x00000400,

    INTERACTION_CONFIGURATION_FLAG_TAP                              = 0x00000001,
    INTERACTION_CONFIGURATION_FLAG_TAP_DOUBLE                       = 0x00000002,

    INTERACTION_CONFIGURATION_FLAG_SECONDARY_TAP                    = 0x00000001,

    INTERACTION_CONFIGURATION_FLAG_HOLD                             = 0x00000001,
    INTERACTION_CONFIGURATION_FLAG_HOLD_MOUSE                       = 0x00000002,

    INTERACTION_CONFIGURATION_FLAG_DRAG                             = 0x00000001,

    INTERACTION_CONFIGURATION_FLAG_CROSS_SLIDE                      = 0x00000001,
    INTERACTION_CONFIGURATION_FLAG_CROSS_SLIDE_HORIZONTAL           = 0x00000002,
    INTERACTION_CONFIGURATION_FLAG_CROSS_SLIDE_SELECT               = 0x00000004,
    INTERACTION_CONFIGURATION_FLAG_CROSS_SLIDE_SPEED_BUMP           = 0x00000008,
    INTERACTION_CONFIGURATION_FLAG_CROSS_SLIDE_REARRANGE            = 0x00000010,
    INTERACTION_CONFIGURATION_FLAG_CROSS_SLIDE_EXACT                = 0x00000020,

    INTERACTION_CONFIGURATION_FLAG_MAX                              = 0xffffffff,
} INTERACTION_CONFIGURATION_FLAGS;

typedef struct INTERACTION_CONTEXT_CONFIGURATION
{
    INTERACTION_ID interactionId;
    INTERACTION_CONFIGURATION_FLAGS enable;

} INTERACTION_CONTEXT_CONFIGURATION;

typedef struct MANIPULATION_TRANSFORM
{
    float translationX;
    float translationY;
    float scale;
    float expansion;
    float rotation;
} MANIPULATION_TRANSFORM;

typedef struct MANIPULATION_VELOCITY
{
    float velocityX;
    float velocityY;
    float velocityExpansion;
    float velocityAngular;
} MANIPULATION_VELOCITY;

typedef enum MANIPULATION_RAILS_STATE
{
    MANIPULATION_RAILS_STATE_UNDECIDED = 0,
    MANIPULATION_RAILS_STATE_FREE = 1,
    MANIPULATION_RAILS_STATE_RAILED = 2,
    MANIPULATION_RAILS_STATE_MAX = 0xffffffff,
} MANIPULATION_RAILS_STATE;

typedef struct INTERACTION_ARGUMENTS_MANIPULATION
{
    MANIPULATION_TRANSFORM delta;
    MANIPULATION_TRANSFORM cumulative;
    MANIPULATION_VELOCITY veolcity;
    MANIPULATION_RAILS_STATE railsState;
} INTERACTION_ARGUMENTS_MANIPULATION;

typedef struct INTERACTION_ARGUMENTS_TAP
{
    UINT32 count;
} INTERACTION_ARGUMENTS_TAP;

typedef enum CROSS_SLIDE_FLAGS
{
    CROSS_SLIDE_FLAGS_NONE       = 0x00000000,
    CROSS_SLIDE_FLAGS_SELECT     = 0x00000001,
    CROSS_SLIDE_FLAGS_SPEED_BUMP = 0x00000002,
    CROSS_SLIDE_FLAGS_REARRANGE  = 0x00000004,
    CROSS_SLIDE_FLAGS_MAX        = 0xffffffff,
} CROSS_SLIDE_FLAGS;

typedef struct INTERACTION_ARGUMENTS_CROSS_SLIDE
{
    CROSS_SLIDE_FLAGS flags;
} INTERACTION_ARGUMENTS_CROSS_SLIDE;

typedef struct INTERACTION_CONTEXT_OUTPUT
{
    INTERACTION_ID interactionId;
    INTERACTION_FLAGS interactionFlags;
    POINTER_INPUT_TYPE inputType;
    float x;
    float y;
    union
    {
        INTERACTION_ARGUMENTS_MANIPULATION manipulation;
        INTERACTION_ARGUMENTS_TAP tap;
        INTERACTION_ARGUMENTS_CROSS_SLIDE crossSlide;
    } arguments;
} INTERACTION_CONTEXT_OUTPUT;

typedef void (CALLBACK *INTERACTION_CONTEXT_OUTPUT_CALLBACK)(void *data,
        const INTERACTION_CONTEXT_OUTPUT *output);

HRESULT WINAPI CreateInteractionContext(HINTERACTIONCONTEXT *context);
HRESULT WINAPI DestroyInteractionContext(HINTERACTIONCONTEXT context);

HRESULT WINAPI ProcessInertiaInteractionContext(HINTERACTIONCONTEXT context);

HRESULT WINAPI RegisterOutputCallbackInteractionContext(HINTERACTIONCONTEXT context,
        INTERACTION_CONTEXT_OUTPUT_CALLBACK callback, void *data);

HRESULT WINAPI GetInteractionConfigurationInteractionContext(HINTERACTIONCONTEXT context,
        UINT32 count, INTERACTION_CONTEXT_CONFIGURATION *configuration);
HRESULT WINAPI SetInteractionConfigurationInteractionContext(HINTERACTIONCONTEXT context,
        UINT32 count, const INTERACTION_CONTEXT_CONFIGURATION *configuration);

HRESULT WINAPI GetPropertyInteractionContext(HINTERACTIONCONTEXT context,
        INTERACTION_CONTEXT_PROPERTY proerty, UINT32 *value);
HRESULT WINAPI SetPropertyInteractionContext(HINTERACTIONCONTEXT context,
        INTERACTION_CONTEXT_PROPERTY property, UINT32 value);

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif
