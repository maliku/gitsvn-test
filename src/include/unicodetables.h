/*!============================================================================
 * @file unicodetables.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

enum UnicodeVersion {
    Unicode_Unassigned,
    Unicode_1_1,
    Unicode_2_0,
    Unicode_2_1_2,
    Unicode_3_0,
    Unicode_3_1,
    Unicode_3_2,
    Unicode_4_0,
    Unicode_4_1,
    Unicode_5_0
};

typedef struct {
    Uint16 category : 8;
    Uint16 line_break_class : 8;
    Uint16 direction : 8;
    Uint16 combiningClass :8;
    Uint16 joining : 2;
    signed short digitValue : 6; /* 5 needed */
    Uint16 unicodeVersion : 4;
    Uint16 lowerCaseSpecial : 1;
    Uint16 upperCaseSpecial : 1;
    Uint16 titleCaseSpecial : 1;
    Uint16 caseFoldSpecial : 1; /* currently unused */
    signed short mirrorDiff : 16;
    signed short lowerCaseDiff : 16;
    signed short upperCaseDiff : 16;
    signed short titleCaseDiff : 16;
    signed short caseFoldDiff : 16;
    Uint16 graphemeBreak : 8;
    Uint16 wordBreak : 8;
    Uint16 sentenceBreak : 8;
} UnicodeProperties;

enum Script {
    Common,
    Greek,
    Cyrillic,
    Armenian,
    Hebrew,
    Arabic,
    Syriac,
    Thaana,
    Devanagari,
    Bengali,
    Gurmukhi,
    Gujarati,
    Oriya,
    Tamil,
    Telugu,
    Kannada,
    Malayalam,
    Sinhala,
    Thai,
    Lao,
    Tibetan,
    Myanmar,
    Georgian,
    Hangul,
    Ogham,
    Runic,
    Khmer,
    Nko,
    Inherited,
    ScriptCount = Inherited,
    Latin = Common,
    Ethiopic = Common,
    Cherokee = Common,
    CanadianAboriginal = Common,
    Mongolian = Common,
    Hiragana = Common,
    Katakana = Common,
    Bopomofo = Common,
    Han = Common,
    Yi = Common,
    OldItalic = Common,
    Gothic = Common,
    Deseret = Common,
    Tagalog = Common,
    Hanunoo = Common,
    Buhid = Common,
    Tagbanwa = Common,
    Limbu = Common,
    TaiLe = Common,
    LinearB = Common,
    Ugaritic = Common,
    Shavian = Common,
    Osmanya = Common,
    Cypriot = Common,
    Braille = Common,
    Buginese = Common,
    Coptic = Common,
    NewTaiLue = Common,
    Glagolitic = Common,
    Tifinagh = Common,
    SylotiNagri = Common,
    OldPersian = Common,
    Kharoshthi = Common,
    Balinese = Common,
    Cuneiform = Common,
    Phoenician = Common,
    PhagsPa = Common
};

