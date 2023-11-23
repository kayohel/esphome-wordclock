#include <cstdint>

namespace Wordclock
{

    struct Word
    {
        // x left to right
        // y bottom to top
        unsigned x, y, length;
    };

    namespace Words
    {

        static const Word hour_words[] = {
            {5, 5, 5}, // HOUR_TWELVE
            {2, 4, 4}, // HOUR_ONE     "EINS"
            {0, 4, 4}, // HOUR_TWO
            {1, 3, 4}, // HOUR_THREE
            {7, 2, 4}, // HOUR_FOUR
            {7, 3, 4}, // HOUR_FIVE
            {1, 0, 5}, // HOUR_SIX
            {5, 4, 6}, // HOUR_SEVEN
            {1, 1, 4}, // HOUR_EIGHT
            {3, 2, 4}, // HOUR_NINE
            {5, 1, 4}, // HOUR_TEN
            {0, 2, 3}, // HOUR_ELEVEN
        };

        const Word EIN{2, 4, 3}; // EIN
        const Word ES{0, 9, 2};
        const Word IST{3, 9, 3};
        const Word UHR{8, 0, 3};
        const Word FUENF{7, 9, 4};
        const Word ZEHN{0, 8, 4};
        const Word VIERTEL{4, 7, 7};
        const Word ZWANZIG{4, 8, 7};
        const Word HALB{0, 5, 4};
        const Word NACH{2, 6, 4};
        const Word VOR{6, 6, 3};

    } // namespace Words

    template <typename SetPixel>
    struct Display
    {
        const unsigned LINE_LENGTH = 11;
        const unsigned LINE_COUNT = 10;

        SetPixel &setPixel;
        Display(SetPixel &sp) : setPixel{sp} {};

        void set_word(const Word &word)
        {

            if (word.x >= 0 && word.x < LINE_LENGTH && word.y >= 0 && word.y < LINE_COUNT)
            {
                // LED strip is a serpentine line
                if (word.y % 2 == 0)
                {
                    unsigned pos = (LINE_LENGTH - 1 - word.x) + word.y * LINE_LENGTH;
                    for (unsigned i = 0; i < word.length; i++)
                    {
                        setPixel(pos - i);
                    }
                }
                else
                {
                    unsigned pos = word.x + word.y * LINE_LENGTH;
                    for (unsigned i = 0; i < word.length; i++)
                    {
                        setPixel(pos + i);
                    }
                }
            }
        }
    };

    struct Time
    {
        unsigned hour{0};
        unsigned minute{0};
    };

    template <typename SetPixel>
    void update(const Time &time, SetPixel &setPixel)
    {
        Display display(setPixel);

        display.set_word(Words::ES);
        display.set_word(Words::IST);

        auto hours = time.hour % 12;
        auto minutes = time.minute - (time.minute % 5);
        if ((time.minute % 5) > 2)
        {
            minutes = minutes + 1;
        }
        if (minutes == 60)
        {
            minutes = 0;
            hours = hours + 1;
        }

        auto parts_of_minutes = minutes / 5;
        switch (parts_of_minutes)
        {
        default:
        case 0:
            display.set_word(Words::UHR);
            break;
        case 1:
            display.set_word(Words::FUENF);
            display.set_word(Words::NACH);
            break;
        case 2:
            display.set_word(Words::ZEHN);
            display.set_word(Words::NACH);
            break;
        case 3:
            display.set_word(Words::VIERTEL);
            display.set_word(Words::NACH);
            break;
        case 4:
            display.set_word(Words::ZWANZIG);
            display.set_word(Words::NACH);
            break;
        case 5:
            display.set_word(Words::FUENF);
            display.set_word(Words::VOR);
            display.set_word(Words::HALB);
            break;
        case 6:
            display.set_word(Words::HALB);
            break;
        case 7:
            display.set_word(Words::FUENF);
            display.set_word(Words::NACH);
            display.set_word(Words::HALB);
            break;
        case 8:
            display.set_word(Words::ZWANZIG);
            display.set_word(Words::VOR);
            break;
        case 9:
            display.set_word(Words::VIERTEL);
            display.set_word(Words::VOR);
            break;
        case 10:
            display.set_word(Words::ZEHN);
            display.set_word(Words::VOR);
            break;
        case 11:
            display.set_word(Words::FUENF);
            display.set_word(Words::VOR);
            break;
        }
        if (parts_of_minutes >= 5)
        {
            hours = (hours + 1) % 12;
        }

        if ((parts_of_minutes == 0) && (hours == 1))
        {
            display.set_word(Words::EIN); // EIN (instead of EINS)
        }
        else
        {
            display.set_word(Words::hour_words[hours]);
        }
    }

} // namespace Wordclock