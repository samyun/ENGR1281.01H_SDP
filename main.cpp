#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#define time 250														//time is number of samples to take

//class prototype
class data{
	public:
		double getData();												//Fn getData will collect the data from the pin and return an array of data with type double
		int calcData(raw[]);											//Fn calcData will calculate the data from the array and return a value
		void showData(int,int);											//Fn showData will display the data received.
	private:
		double raw[1000];
		int freq;
		int checkFreq;
}

//Define Proteus vars
ButtonBoard buttons( FEHIO::Bank3 );
DigitalInputPin pin( FEHIO::P0_0);

int main(void)
{
    //Define variables
	int stop = 0;															//"stop" will be the exit for the loops while checking for the middle button.

	//Print greeting message
    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );
	LCDWriteLine("Press middle button to start collection.");
	
	//Wait until middle is pressed
	while (!stop){
		Sleep(100);														//Conserve power by delaying the check by 0.1 seconds.
		if (buttons.MiddlePressed()){
			stop = 1;
		}
	}
	
	//reset stop variable 
    stop = 0;
	
	//run showData function to put something on screen while doing calculation
	data.showData();
	
	//start loop for collection
	while (!stop){
		data.getData();
		data.calcData();
		data.showData();
		
		//exit loop if button is pressed
		if (buttons.MiddlePressed()){
			stop = 1;
		}
	}
	
	//Closing message
	LCD.Clear();
	LCD.WriteLine("Goodbye.");
}

//define getData function
double data::getData(raw[]){
	for (int i = 0; i < time; i++){
		raw[i] = pin.Value();
}

//define calcData function
int data::calcData(raw[]){
	
}

//define showData function
void data::showData{
	LCD.Clear();
	LCD.WriteLine("Press middle button to stop collection");
	LCD.Write(freq);
	LCD.WriteLine(" Hz");
}