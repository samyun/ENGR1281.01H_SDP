/**
 *ENGR 1281.01H AU13 Software Design Project
 *Created by Sam Yun & Isaac McDermott
 *
 *This is a C/C++ program designed to be loaded onto an OSU FEH Proteus
 *microcontroller to display frequencies colelcted. The Proteus must
 *have an IR receiver and a button board connected for the program to
 *function. The program initially prompts the user to press the middle
 *button, and then it will display one of 5 known values: 25 Hz, 40 Hz,
 *65 Hz, 80 Hz, 110 Hz. It will display 0 Hz if an unknown signal or no
 *signal is received. When the middle button is pressed again, it will
 *display a closing message.
 */

/**
 *Necessary headers
 *
 *<FEHLCD.h> contains the libraries to control the LCD on the Proteus.
 *<FEHIO.h> contains the libraries to input the signals from the button
 *board and LCD.
 *<FEHUtility.h> contains the libraries for other functions, such as timing.
 *time *** defines the number of samples that should be taken while
 *calculating the frequency. This number roughly corresponds to the number
 *of milliseconds in the period.
 */
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#define time 250

/**
 *The data class
 *
 *The data class contains various private variables and public functions.
 *"raw" is an int that will contain the raw 1/0 values taken from the IR
 *receiver. "freq" is an int that represents the filtered calculated
 *frequency. "k" is an int that represents the number of switches from 0 to
 *1. "fraw" is a float that represents the raw, pre-filtered frequency.
 *"check" is a float that represents a check for the calculated frequency.
 *"freqold" is a float representing the previous frequency.
 *
 *The "data" default constructor will set the default values of all
 *variables to 0. The "getData" function will collect the data from the pin
 *and return an array of data with type float. The "calcData" function will
 *display the data calculated.
 */
class data
{
    private:
        int raw[1000];
        int freq;
        int k;
        float fraw;
        float check;
        float freqold;
    public:
        data (int b = 0, int c = 0, int d = 0, float e = 0, float f = 0, float g = 1);
        void getData();
        void calcData();
        void showData();
};

/**
 *Pin variables
 *
 *"button" is mapped to Pin P3_1 as a digital input. "pin" is mapped to Pin
 *P0_0 as a digital input.
 */
DigitalInputPin button(FEHIO::P3_1);
DigitalInputPin pin(FEHIO::P0_0);


/**
 *The main function
 *
 *This function first creates a "stop" variable which will be the exit for
 *the future loops and an instance of the data class called "data". Then it
 *clears the LCD, sets it to have a black background and a white font color.
 *Then it writes a greeting message.
 *
 *After waiting for the middle button to be pressed, the showData function
 *is run, showing data.
 */
int main(void)
{
    //Define variables
    int stop = 0;
    data data;

    //Print greeting message
    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );
    LCD.WriteLine("     Infrared Reader     ");
    LCD.WriteLine("  Press middle button to ");
    LCD.WriteLine("     start collection.   ");

    //Wait until middle is pressed
    while (button.Value()){}

    //reset stop variable
    stop = 0;

    //run showData function to put something on screen while doing calculation
    data.showData();

    /**
     *Start loop for collection.
     *While in the loop, run the 3 functions to:
     *  get raw data from the IR receiver,
     *  calculate the frequency,
     *  display the frequency.
     *
     *Then check if button was pressed.
     *If it was pressed, "stop" is true, which ends the function.
     */
    while (!stop)
    {
        data.getData();
        data.calcData();
        if (!button.Value())
        {
            stop = 1;
        }
        data.showData();
    }

    //Closing message
    LCD.Clear();
    LCD.WriteLine("          Goodbye.        ");

    return 0;
}

/**
 *The data constructor.
 *
 *Set all variables on "data" instance of "data" class to 0
 */
data::data(int b, int c, int d, float e, float f, float g)
{
    for (int i = 0; i < 1000; i++)
    {
        raw[i] = b;
    }
    freq = c;
    k = d;
    fraw = e;
    check = f;
    freqold = g;
}

/**
 *The getData function.
 *
 *Populate "raw" array with 1/0 value of "pin", sleeping for 1 ms in
 *between. If the value "pin" inputs is true, a 1 is written. If it is
 *false, a 0 is written.
 *
 *Doesn't return anything.
 */
void data::getData()
{
    //populate raw array.
    for (int i = 0; i < time; i++)
    {
        if (pin.Value())
        {
            raw[i] = 1;
        }else
        {
            raw[i] = 0;
        }
        Sleep (1);
    }
}

/**
 *The calcData function
 *
 *The calcData function runs the calculations on the raw data collected from
 *the getData function. First, the function counts the number of times the
 *values in the "raw" array switches from 0 to 1. A raw frequency is then
 *calculated.
 *
 *Then the raw frequency is filtered into the 5 known frequencies. First it
 *is checked whether is raw frequency is within the range of +- 3.5 Hz. If
 *it is not, the program continues. If it is, it checks whether the raw
 *frequency is within 3 Hz or 3% (whichever is greater) of a known value
 *(other than the 80 Hz, which checks within -4 and +3 Hz). If it is, it
 *sets freq to be the expected value. Otherwise, it sets freq to be 0.
 *
 *Finally it sets the check to be the raw frequency.
 *
 *Doesn't return anything.
 */
void data::calcData()
{
    //count number of switches from 0 to 1
    k = 0;
    for (int i = 0; i < time; i++)
    {
        if (raw[i] == 0 && raw[i+1] == 1)
        {
            k++;
        }
    }

    fraw = (k * 1000) / time;   //calculate raw frequency

    //filter frequencies
    if (fraw >= (check - 3.5) && fraw <= (check + 3.5))
    {
        if (fraw >= 22 && fraw <= 28)
        {
            freq = 25;
        }
        else if (fraw >= 37 && fraw <= 43)
        {
            freq = 40;
        }
        else if (fraw >= 62 && fraw <= 68)
        {
            freq = 65;
        }
        else if (fraw >= 76 && fraw <= 83)
        {
            freq = 80;
        }
        else if (fraw >= 106.7 && fraw <= 113.3)
        {
            freq = 110;
        }
        else
        {
            freq = 0;
        }
    }

    //set check
    check = fraw;
}



/**
 *define showData function
 *
 *The showData function will first check is the old frequency is equal to
 *the new frequency. If it is, it sets the old frequency to be the new
 *frequency and exits. Otherwise, it clears the LCD and displays the
 *instruction to press the middle button to quit and the new frequency.
 *Then it sets the old frequency to be the new frequency.
 *
 *Doesn't return anything.
 */
void data::showData()
{
    //check if old freq == new freq
    if (freqold != freq)
    {
        LCD.Clear();
        LCD.WriteLine("  Press middle button to  ");
        LCD.WriteLine("      stop collection     ");
        LCD.WriteLine(" ");
        LCD.Write("           ");
        LCD.Write(freq);
        LCD.WriteLine(" Hz");
    }

    //set old freq to be new freq
    freqold = freq;
}
