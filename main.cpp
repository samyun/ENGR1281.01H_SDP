/*ENGR 1281.01H AU13 Software Design Project
 *Created by Sam Yun & Isaac McDermott
 *
 *This program is designed to be loaded onto an OSU FEH Proteus microcontroller.
 *The Proteus must have an IR receiver and a button board connected for the program
 *to function. The program initially prompts the user to press the middle button,
 *and then it will display one of 5 known values: 25 Hz, 40 Hz, 65 Hz, 80 Hz, 110 Hz.
 *It will display 0 Hz if an unknown signal or no signal is received. When the middle
 *button is pressed again, it will display a closing message.
 */

/*Necessary headers
 *
 *<FEHLCD.h> contains the libraries to control the LCD on the Proteus.
 *<FEHIO.h> contains the libraries to input the signals from the button board and LCD.
 *<FEHUtility.h> contains the libraries for other functions, such as timing.
 *time *** defines the number of samples that should be taken while calculating the
 *frequency. This number roughly corresponds to the number of milliseconds in the period.
 */
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#define time 500

//class prototype
class data
{
    private:
        int raw[1000];
        int freq;
        int k;
        int i;
        double fraw;
        double fraw1;
    public:
        data (int a=0, int b=0, int c=0, int d=0, double e=0, double f=0);     //Sets default constructor to 0s.
        void getData();          //Fn getData will collect the data from the pin and return an array of data with type double
        void calcData();            //Fn calcData will calculate the data from the array and return a value
        void showData();              //Fn showData will display the data received.
};

//Define Proteus vars
DigitalInputPin button(FEHIO::P3_1);    //Define middle button
DigitalInputPin pin(FEHIO::P0_0);       //Define IR reader's pin

int main(void)
{
    //Define variables
    int stop = 0;       //"stop" will be the exit for the loops while checking for the middle button.
    data data;          //define instance of data class

    //Print greeting message
    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );
    LCD.WriteLine("Infrared Reader");
    LCD.WriteLine("Press middle button to ");
    LCD.WriteLine("start collection.");

    //Wait until middle is pressed
    while (button.Value()){}

    //reset stop variable
    stop = 0;

    //run showData function to put something on screen while doing calculation
    data.showData();

    /*Start loop for collection.
      While in the loop, run the 3 functions to:
        get raw data from the IR receiver,
        calculate the frequency,
        display the frequency.

      Then check if button was pressed.
      If it was pressed, "stop" is true, which ends the function.*/
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
    LCD.WriteLine("Goodbye.");

    return 0;
}

//define constructor: Set all variables on "data" instance of "data" class to 0
data::data(int a, int b, int c, int d, double e, double f)
{
    i = a;
    for (i=0; i<1000; i++)
    {
        raw[i]=b;
    }
    freq=c;
    k=d;
    fraw=e;
    fraw1=f;
}

/*define getData function:
  Fill "raw" array with value of "pin", sleeping for 1 ms*/
void data::getData()
{
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

/*define calcData function
 *
 *The calcData function will first determine how many times in the raw array
 *the data is false.
  */
void data::calcData()
{
    k = 0;
    for (int i = 0; i < time; i++)
    {
        if (raw[i] == 0 && raw[i+1] == 1)
        {
            k++;
        }
    }

    fraw = (k * 1000) / time;

    if (fraw >= (fraw1 - 3.5) && fraw <= (fraw1 + 3.5))
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
        else if (fraw >= 77 && fraw <= 83)
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
    }else
    {
        freq = freq;
    }
    fraw1 = fraw;
}



/*define showData function
 *
 *showData function will first clear the screen. Then it will write the line
 *"Press the middle button to stop collection" and then on the next line,
 *write the frequency and then "Hz".
 */
void data::showData(){
        LCD.Clear();
        LCD.WriteLine("Press middle button to ");
        LCD.WriteLine("stop collection");
        LCD.Write(freq);
        LCD.WriteLine(" Hz");
}
