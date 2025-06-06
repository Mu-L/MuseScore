/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore Limited
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <set>

#include "global/containers.h"

#include "../types/types.h"
#include "../style/styledef.h"

#include "property.h"

namespace mu::engraving {
class EngravingItem;

enum class CommandType : signed char {
    Unknown = -1,

    // Parts
    InsertPart,
    RemovePart,
    AddPartToExcerpt,
    SetSoloist,
    ChangePart,

    // Staves
    InsertStaff,
    RemoveStaff,
    AddSystemObjectStaff,
    RemoveSystemObjectStaff,
    SortStaves,
    ChangeStaff,
    ChangeStaffType,

    // MStaves
    InsertMStaff,
    RemoveMStaff,
    InsertStaves,
    RemoveStaves,
    ChangeMStaffProperties,

    // Instruments
    ChangeInstrumentShort,
    ChangeInstrumentLong,
    ChangeInstrument,
    ChangeDrumset,

    // Measures
    RemoveMeasures,
    InsertMeasures,
    ChangeMeasureLen,
    ChangeMMRest,
    ChangeMeasureRepeatCount,

    // Elements
    AddElement,
    RemoveElement,
    Unlink,
    Link,
    ChangeElement,
    ChangeParent,

    // Notes
    ChangePitch,
    ChangeFretting,
    ChangeVelocity,

    // ChordRest
    ChangeChordStaffMove,
    SwapCR,

    // Brackets
    RemoveBracket,
    AddBracket,

    // Fret
    FretDataChange,
    FretDot,
    FretMarker,
    FretBarre,
    FretClear,
    FretLinkHarmony,

    // FBox
    ReorderFBox,
    RenameChordFBox,
    AddChordFBox,
    RemoveChordFBox,

    // Harmony
    TransposeHarmony,

    // KeySig
    ChangeKeySig,

    // Clef
    ChangeClefType,

    // Tremolo
    MoveTremolo,

    // Spanners
    ChangeSpannerElements,
    InsertTimeUnmanagedSpanner,
    ChangeStartEndSpanner,

    // Ties
    ChangeTieEndPointActive,

    // Style
    ChangeStyle,
    ChangeStyleValues,

    // Property
    ChangeProperty,

    // Voices
    ExchangeVoice,
    CloneVoice,

    // Excerpts
    AddExcerpt,
    RemoveExcerpt,
    SwapExcerpt,
    ChangeExcerptTitle,

    // Meta info
    ChangeMetaInfo,

    // Other
    InsertTime,
    ChangeScoreOrder,
};

struct NoteInputParams {
    int step = 0;
    int drumPitch = -1;
};

//---------------------------------------------------------
//   AccidentalType
//---------------------------------------------------------
// NOTE: keep this in sync with accList array in accidentals.cpp

enum class AccidentalType : unsigned char {
    ///.\{
    NONE,
    FLAT,
    NATURAL,
    SHARP,
    SHARP2,
    FLAT2,
    SHARP3,
    FLAT3,
    NATURAL_FLAT,
    NATURAL_SHARP,
    SHARP_SHARP,

    // Gould arrow quartertone
    FLAT_ARROW_UP,
    FLAT_ARROW_DOWN,
    NATURAL_ARROW_UP,
    NATURAL_ARROW_DOWN,
    SHARP_ARROW_UP,
    SHARP_ARROW_DOWN,
    SHARP2_ARROW_UP,
    SHARP2_ARROW_DOWN,
    FLAT2_ARROW_UP,
    FLAT2_ARROW_DOWN,
    ARROW_DOWN,
    ARROW_UP,

    // Stein-Zimmermann
    MIRRORED_FLAT,
    MIRRORED_FLAT2,
    SHARP_SLASH,
    SHARP_SLASH4,

    // Arel-Ezgi-Uzdilek (AEU)
    FLAT_SLASH2,
    FLAT_SLASH,
    SHARP_SLASH3,
    SHARP_SLASH2,

    // Extended Helmholtz-Ellis accidentals (just intonation)
    DOUBLE_FLAT_ONE_ARROW_DOWN,
    FLAT_ONE_ARROW_DOWN,
    NATURAL_ONE_ARROW_DOWN,
    SHARP_ONE_ARROW_DOWN,
    DOUBLE_SHARP_ONE_ARROW_DOWN,
    DOUBLE_FLAT_ONE_ARROW_UP,

    FLAT_ONE_ARROW_UP,
    NATURAL_ONE_ARROW_UP,
    SHARP_ONE_ARROW_UP,
    DOUBLE_SHARP_ONE_ARROW_UP,
    DOUBLE_FLAT_TWO_ARROWS_DOWN,
    FLAT_TWO_ARROWS_DOWN,

    NATURAL_TWO_ARROWS_DOWN,
    SHARP_TWO_ARROWS_DOWN,
    DOUBLE_SHARP_TWO_ARROWS_DOWN,
    DOUBLE_FLAT_TWO_ARROWS_UP,
    FLAT_TWO_ARROWS_UP,
    NATURAL_TWO_ARROWS_UP,

    SHARP_TWO_ARROWS_UP,
    DOUBLE_SHARP_TWO_ARROWS_UP,
    DOUBLE_FLAT_THREE_ARROWS_DOWN,
    FLAT_THREE_ARROWS_DOWN,
    NATURAL_THREE_ARROWS_DOWN,
    SHARP_THREE_ARROWS_DOWN,

    DOUBLE_SHARP_THREE_ARROWS_DOWN,
    DOUBLE_FLAT_THREE_ARROWS_UP,
    FLAT_THREE_ARROWS_UP,
    NATURAL_THREE_ARROWS_UP,
    SHARP_THREE_ARROWS_UP,
    DOUBLE_SHARP_THREE_ARROWS_UP,

    LOWER_ONE_SEPTIMAL_COMMA,
    RAISE_ONE_SEPTIMAL_COMMA,
    LOWER_TWO_SEPTIMAL_COMMAS,
    RAISE_TWO_SEPTIMAL_COMMAS,
    LOWER_ONE_UNDECIMAL_QUARTERTONE,
    RAISE_ONE_UNDECIMAL_QUARTERTONE,

    LOWER_ONE_TRIDECIMAL_QUARTERTONE,
    RAISE_ONE_TRIDECIMAL_QUARTERTONE,

    DOUBLE_FLAT_EQUAL_TEMPERED,
    FLAT_EQUAL_TEMPERED,
    NATURAL_EQUAL_TEMPERED,
    SHARP_EQUAL_TEMPERED,
    DOUBLE_SHARP_EQUAL_TEMPERED,
    QUARTER_FLAT_EQUAL_TEMPERED,
    QUARTER_SHARP_EQUAL_TEMPERED,

    FLAT_17,
    SHARP_17,
    FLAT_19,
    SHARP_19,
    FLAT_23,
    SHARP_23,
    FLAT_31,
    SHARP_31,
    FLAT_53,
    SHARP_53,
    EQUALS_ALMOST,
    EQUALS,
    TILDE,

    // Persian
    SORI,
    KORON,

    // Wyschnegradsky
    TEN_TWELFTH_FLAT,
    TEN_TWELFTH_SHARP,
    ELEVEN_TWELFTH_FLAT,
    ELEVEN_TWELFTH_SHARP,
    ONE_TWELFTH_FLAT,
    ONE_TWELFTH_SHARP,
    TWO_TWELFTH_FLAT,
    TWO_TWELFTH_SHARP,
    THREE_TWELFTH_FLAT,
    THREE_TWELFTH_SHARP,
    FOUR_TWELFTH_FLAT,
    FOUR_TWELFTH_SHARP,
    FIVE_TWELFTH_FLAT,
    FIVE_TWELFTH_SHARP,
    SIX_TWELFTH_FLAT,
    SIX_TWELFTH_SHARP,
    SEVEN_TWELFTH_FLAT,
    SEVEN_TWELFTH_SHARP,
    EIGHT_TWELFTH_FLAT,
    EIGHT_TWELFTH_SHARP,
    NINE_TWELFTH_FLAT,
    NINE_TWELFTH_SHARP,

    // (Spartan) Sagittal
    SAGITTAL_5V7KD,
    SAGITTAL_5V7KU,
    SAGITTAL_5CD,
    SAGITTAL_5CU,
    SAGITTAL_7CD,
    SAGITTAL_7CU,
    SAGITTAL_25SDD,
    SAGITTAL_25SDU,
    SAGITTAL_35MDD,
    SAGITTAL_35MDU,
    SAGITTAL_11MDD,
    SAGITTAL_11MDU,
    SAGITTAL_11LDD,
    SAGITTAL_11LDU,
    SAGITTAL_35LDD,
    SAGITTAL_35LDU,
    SAGITTAL_FLAT25SU,
    SAGITTAL_SHARP25SD,
    SAGITTAL_FLAT7CU,
    SAGITTAL_SHARP7CD,
    SAGITTAL_FLAT5CU,
    SAGITTAL_SHARP5CD,
    SAGITTAL_FLAT5V7KU,
    SAGITTAL_SHARP5V7KD,
    SAGITTAL_FLAT,
    SAGITTAL_SHARP,

    // Turkish folk music
    ONE_COMMA_FLAT,
    ONE_COMMA_SHARP,
    TWO_COMMA_FLAT,
    TWO_COMMA_SHARP,
    THREE_COMMA_FLAT,
    THREE_COMMA_SHARP,
    FOUR_COMMA_FLAT,
    //FOUR_COMMA_SHARP,
    FIVE_COMMA_SHARP,

    END
    ///\}
};

//---------------------------------------------------------
//   NoteType
//---------------------------------------------------------

enum class NoteType : unsigned char {
    ///.\{
    NORMAL        = 0,
    ACCIACCATURA  = 0x1,
    APPOGGIATURA  = 0x2,         // grace notes
    GRACE4        = 0x4,
    GRACE16       = 0x8,
    GRACE32       = 0x10,
    GRACE8_AFTER  = 0x20,
    GRACE16_AFTER = 0x40,
    GRACE32_AFTER = 0x80,
    INVALID       = 0xFF
                    ///\}
};

constexpr NoteType operator|(NoteType t1, NoteType t2)
{
    return static_cast<NoteType>(static_cast<unsigned char>(t1) | static_cast<unsigned char>(t2));
}

constexpr bool operator&(NoteType t1, NoteType t2)
{
    return static_cast<unsigned char>(t1) & static_cast<unsigned char>(t2);
}

//---------------------------------------------------------
//   HarmonyType
//---------------------------------------------------------

enum class HarmonyType : unsigned char {
    ///.\{
    STANDARD,
    ROMAN,
    NASHVILLE
    ///\}
};

//---------------------------------------------------------
//   MMRestRangeBracketType
//---------------------------------------------------------

enum class MMRestRangeBracketType : unsigned char {
    ///.\{
    BRACKETS, PARENTHESES, NONE
    ///\}
};

//---------------------------------------------------------
//   OffsetType
//---------------------------------------------------------

enum class OffsetType : unsigned char {
    ABS,         ///< offset in point units
    SPATIUM      ///< offset in staff space units
};

//-------------------------------------------------------------------
//   SegmentType
//
//    Type values determine the order of segments for a given tick
//-------------------------------------------------------------------

enum class SegmentType {
    ///.\{
    Invalid               = 0x0,
    BeginBarLine          = 0x1,
    HeaderClef            = 0x2,
    KeySig                = 0x4,
    Ambitus               = 0x8,
    TimeSig               = 0x10,
    StartRepeatBarLine    = 0x20,
    ClefStartRepeatAnnounce    = 0x40,
    KeySigStartRepeatAnnounce  = 0x80,
    TimeSigStartRepeatAnnounce = 0x100,
    Clef                  = 0x200,
    BarLine               = 0x400,
    Breath                = 0x800,
    //--
    TimeTick              = 0x1000,
    ChordRest             = 0x2000,
    //--
    ClefRepeatAnnounce    = 0x4000,
    KeySigRepeatAnnounce  = 0x8000,
    TimeSigRepeatAnnounce = 0x10000,
    //--
    EndBarLine            = 0x20000,
    KeySigAnnounce        = 0x40000,
    TimeSigAnnounce       = 0x80000,
    //--
    All                   = -1,   ///< Includes all barline types
    /// Alias for `BeginBarLine | StartRepeatBarLine | BarLine | EndBarLine`
    BarLineType           = BeginBarLine | StartRepeatBarLine | BarLine | EndBarLine,
    CourtesyTimeSigType   = TimeSigAnnounce | TimeSigRepeatAnnounce | TimeSigStartRepeatAnnounce,
    CourtesyKeySigType    = KeySigAnnounce | KeySigRepeatAnnounce | KeySigStartRepeatAnnounce,
    CourtesyClefType      = ClefRepeatAnnounce | ClefStartRepeatAnnounce,
    TimeSigType           = TimeSig | CourtesyTimeSigType,
    KeySigType            = KeySig | CourtesyKeySigType,
    ClefType              = Clef | HeaderClef | CourtesyClefType,
    ///\}
};

constexpr SegmentType operator|(const SegmentType t1, const SegmentType t2)
{
    return static_cast<SegmentType>(static_cast<int>(t1) | static_cast<int>(t2));
}

constexpr bool operator&(const SegmentType t1, const SegmentType t2)
{
    return static_cast<int>(t1) & static_cast<int>(t2);
}

//---------------------------------------------------------
//   FontStyle
//---------------------------------------------------------

enum class FontStyle : signed char {
    Undefined = -1,
    Normal = 0,
    Bold = 1 << 0,
    Italic = 1 << 1,
    Underline = 1 << 2,
    Strike = 1 << 3
};

constexpr FontStyle operator+(FontStyle a1, FontStyle a2)
{
    return static_cast<FontStyle>(static_cast<char>(a1) | static_cast<char>(a2));
}

constexpr FontStyle operator-(FontStyle a1, FontStyle a2)
{
    return static_cast<FontStyle>(static_cast<char>(a1) & ~static_cast<char>(a2));
}

constexpr bool operator&(FontStyle a1, FontStyle a2)
{
    return static_cast<bool>(static_cast<char>(a1) & static_cast<char>(a2));
}

//---------------------------------------------------------
//   PlayEventType
/// Determines whether ornaments are automatically generated
/// when playing a score and whether the PlayEvents are saved
/// in the score file.
//---------------------------------------------------------

enum class PlayEventType : unsigned char {
    ///.\{
    Auto,         ///< Play events for all notes are calculated by MuseScore.
    User,         ///< Some play events are modified by user. Those events are written into the mscx file.
    ///.\}
};

//---------------------------------------------------------
//   Tuplets
//---------------------------------------------------------

enum class TupletNumberType : unsigned char {
    SHOW_NUMBER, SHOW_RELATION, NO_TEXT
};
enum class TupletBracketType : unsigned char {
    AUTO_BRACKET, SHOW_BRACKET, SHOW_NO_BRACKET
};

//---------------------------------------------------------
//   TripletFeels
//---------------------------------------------------------

enum class TripletFeelType : unsigned char {
    NONE,
    TRIPLET_8TH,
    TRIPLET_16TH,
    DOTTED_8TH,
    DOTTED_16TH,
    SCOTTISH_8TH,
    SCOTTISH_16TH
};

enum class GuitarBendType : unsigned char {
    BEND,
    PRE_BEND,
    GRACE_NOTE_BEND,
    SLIGHT_BEND,
};

enum class GuitarBendShowHoldLine : unsigned char {
    AUTO,
    SHOW,
    HIDE,
};

struct ScoreChangesRange {
    int tickFrom = -1;
    int tickTo = -1;
    staff_idx_t staffIdxFrom = muse::nidx;
    staff_idx_t staffIdxTo = muse::nidx;

    std::map<EngravingItem*, std::unordered_set<CommandType> > changedItems;
    ElementTypeSet changedTypes;
    PropertyIdSet changedPropertyIdSet;
    StyleIdSet changedStyleIdSet;

    bool isValidBoundary() const
    {
        bool tickRangeValid = (tickFrom != -1 && tickTo != -1);
        bool staffRangeValid = (staffIdxFrom != muse::nidx && staffIdxTo != muse::nidx);

        return tickRangeValid && staffRangeValid;
    }

    bool isValid() const
    {
        return isValidBoundary() || !changedTypes.empty();
    }

    void clear()
    {
        *this = ScoreChangesRange();
    }

    void combine(const ScoreChangesRange& r)
    {
        tickFrom = std::min(tickFrom, r.tickFrom);
        tickTo = std::max(tickTo, r.tickTo);
        staffIdxFrom = std::min(staffIdxFrom, r.staffIdxFrom);
        staffIdxTo = std::max(staffIdxTo, r.staffIdxTo);
        changedItems.insert(r.changedItems.begin(), r.changedItems.end());
        changedTypes.insert(r.changedTypes.begin(), r.changedTypes.end());
        changedPropertyIdSet.insert(r.changedPropertyIdSet.begin(), r.changedPropertyIdSet.end());
        changedStyleIdSet.insert(r.changedStyleIdSet.begin(), r.changedStyleIdSet.end());
    }
};
} // namespace mu::engraving

#ifndef NO_QT_SUPPORT
Q_DECLARE_METATYPE(mu::engraving::NoteType)
Q_DECLARE_METATYPE(mu::engraving::PlayEventType)
Q_DECLARE_METATYPE(mu::engraving::AccidentalType)
#endif
