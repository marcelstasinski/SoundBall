/*
  ==============================================================================

    This file was auto-generated!
 
Y_GRACZA = int( WYSOKOSC_LINI - 3 * PROMIEN_GRACZA -(160.f/log2(40.f))*(log2(iter_skok)));
 
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include<vector>
#include<queue>

#define PROMIEN_GRACZA 10
#define LICZBA_PRZESZKOD 2
#define SPREZYSTOSC 4
int X_GRACZA = 0;
int Y_GRACZA = 0;
int WYSOKOSC_LINI = 0;
int GRUBOSC_LINI = 5;
int WYSOKOSC_SKOKU = 100; //wartosc suwaka
const int PREDKOSC_OBIEKTOW = 2;
bool czySkok = true;
bool czyKlikniety = false;



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent, public AnimatedAppComponent, public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        setFramesPerSecond (60);
        
        inicjalizacja();
        addAndMakeVisible (button);
        button.addListener(this);
        
        
        
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void update() override
    {

    }
    
    void paint (Graphics& g) override
    {
//        const int AKTUALNA = sprawdzator_Przeszkod.front();
//        if( Przeszkody [ AKTUALNA ] .czyKolizja( , g ))
        
        
//        for( int i = 0 ; i < LICZBA_PRZESZKOD; ++i )
//            Przeszkody[ i ].czyKolizja( g );
        
        
        
        
        if (czyKlikniety)
        {
            if (czySkok)
            {
                gracz.ruch();
                rysujTlo(g, 0);
                gracz.rysuj(g, 0);
                
            }else
            {
                
                rysujTlo(g, iter_splas);
                gracz.splaszczanie(g);
                ++iter_splas;
                iter_splas%=SPREZYSTOSC;
                
            }
        }
        else
        {
            g.fillAll(Colours::black);
            g.setColour(Colours::white);
            g.drawLine(0, WYSOKOSC_LINI, getWidth(), WYSOKOSC_LINI, 5);
            g.drawEllipse(conv(400, 20), conv( WYSOKOSC_LINI - 2 * PROMIEN_GRACZA , 20), 20, 20, 20);
        }
        
//        g.setColour(Colours::black);
//        g.drawLine(1000 - ruchDziura, WYSOKOSC_LINI + iter_splas/SPREZYSTOSC, 1100 - ruchDziura, WYSOKOSC_LINI + iter_splas/SPREZYSTOSC, 8);
//        ruchDziura += 2;
//        if( ruchDziura > 1000 )
//        {
//            ruchDziura = 0;
//        }
        
        // Rysowanie i ruch przeszkod
        for( int i = 0; i < LICZBA_PRZESZKOD; ++i )
        {
            Przeszkody[ i ].rysuj( g , iter_splas );
            Przeszkody[ i ].ruch();
        }
        // You can add your drawing code here!
    }

    void resized() override
    {
        button.setBounds(100, 100, 50, 50);
        X_GRACZA = getWidth() / 2;
        Y_GRACZA = getHeight() / 2;
        WYSOKOSC_LINI = getHeight() * ( 2.f / 3);
    }
    
    void buttonClicked (Button* button) override
    {
        czyKlikniety = true;
    }

    void zmianaZnaku(bool znak, int& i)
    {
        if (znak == true) i++;
        else i--;
    }
    
    double logarytm(double a, double b )
    {
        return log2(a)/log2(b);
    }
    
    int conv(int x, int r)
    {
        return x-r/2;
    }
    
    void inicjalizacja()
    {
        std::srand(unsigned(std::time(0)));
        
        WYSOKOSC_LINI = getHeight() * ( 2.f / 3);
        gracz = Gracz { getWidth()/2, WYSOKOSC_LINI - PROMIEN_GRACZA*2, PROMIEN_GRACZA*2, PROMIEN_GRACZA*2 };
        
        
        
        const int PRZESUNIECIE_ZA_EKRAN = 300;
        for( int i = 0; i < LICZBA_PRZESZKOD; ++i )
        {
            Przeszkody.push_back( Przeszkoda(getWidth() + i * PRZESUNIECIE_ZA_EKRAN, WYSOKOSC_LINI, 50, 100 ) );
            sprawdzator_Przeszkod.push( i );
            
        }
    }
    
    void rysujTlo(Graphics& grafik, int iterAnim)
    {
        grafik.fillAll(Colours::black);
        grafik.setColour(Colours::white);
        grafik.drawLine(0, WYSOKOSC_LINI+iterAnim, getWidth(), WYSOKOSC_LINI+iterAnim, GRUBOSC_LINI);
    }
    
    class Obiekt
    {
    public:
        virtual void ruch() = 0;
        virtual bool czyKolizja() = 0;
        virtual void rysuj( Graphics& grafik, int iterAnim ) = 0;
        
        virtual ~Obiekt() {}
        
    protected:
        int x;
        int y;
        int szerokosc;
        int wysokosc;
    };
    
    class Gracz : public Obiekt
    {
    public:
        
        Gracz() {}
        Gracz(int xx1, int xx2, int xdeltaX, int xdeltaY)
        {
            x = xx1;
            y = xx2;
            szerokosc = xdeltaX;
            wysokosc = xdeltaY;
        }
        
        void ruch( ) override
        {
            if (WYSOKOSC_LINI - y - PROMIEN_GRACZA == WYSOKOSC_SKOKU)
                czyDoGory = false;
            if (WYSOKOSC_LINI+1 == y + PROMIEN_GRACZA*2)
                czySkok = false;
             
            if (czyDoGory == true) --y;
            else ++y;

        }
        
        bool czyKolizja() override
        {
            return false;
        }
        
        void rysuj( Graphics& grafik, int iterAnim ) override
        {
            grafik.drawEllipse(x - PROMIEN_GRACZA,y - PROMIEN_GRACZA, wysokosc, szerokosc, PROMIEN_GRACZA*2);
            
           // g.drawEllipse(conv(400-iter_splas/2, 20), int( WYSOKOSC_LINI - 3 * PROMIEN_GRACZA + iter_splas), 20 + iter_splas, 20 - iter_splas, 20);
        }
    
        void splaszczanie( Graphics& grafik )
        {
            if (czyDoGory == true) --y;
            else ++y;
            
            if (WYSOKOSC_LINI - y ==  PROMIEN_GRACZA - SPREZYSTOSC/2)
                czyDoGory = true;
            if (WYSOKOSC_LINI+1 == y + PROMIEN_GRACZA*2)
            {
                czySkok = true;
                czyKlikniety = false;
            }
            
           // grafik.drawEllipse(x - PROMIEN_GRACZA,y - PROMIEN_GRACZA + (WYSOKOSC_LINI - y)/2, wysokosc + (WYSOKOSC_LINI - y), szerokosc - (WYSOKOSC_LINI - y), PROMIEN_GRACZA*2);
            
            grafik.drawEllipse(x - PROMIEN_GRACZA, int( WYSOKOSC_LINI - 3 * PROMIEN_GRACZA + (WYSOKOSC_LINI - y)), 20 + (WYSOKOSC_LINI - y), 20 - (WYSOKOSC_LINI - y), 20);
        }
        
        virtual ~Gracz() override {}
        
        bool czyDoGory = true;
        bool flagaSkoku = true;
    };
    
    
    class Przeszkoda : public Obiekt
    {
    public:
        Przeszkoda(int xx1, int xx2, int xdeltaX, int xdeltaY)
        {
            x = xx1;
            y = xx2 - xdeltaY;
            szerokosc = xdeltaX;
            wysokosc = xdeltaY;
        }
        void ruch() override
        {
            const int KONIEC_EKRANU = 0;
            
            x -= PREDKOSC_OBIEKTOW;
            if ( x == KONIEC_EKRANU )
                szerokosc -= PREDKOSC_OBIEKTOW;
        }
        
        bool czyKolizja() override
        {
            bool dotkniecieLewaSciany = x == X_GRACZA + PROMIEN_GRACZA && Y_GRACZA > y;
            bool spadekNaDach = Y_GRACZA + PROMIEN_GRACZA == x  && x < X_GRACZA && X_GRACZA < x + szerokosc;
            bool zahaczenieLewyRog = pow( x - X_GRACZA, 2 ) + pow( y - Y_GRACZA + PROMIEN_GRACZA, 2 ) == pow( PROMIEN_GRACZA, 2 );
            
            if( dotkniecieLewaSciany ||
                spadekNaDach ||
                zahaczenieLewyRog )
            {
                return true;
            }
            
            return false;
        }
        
        void rysuj(Graphics& grafik, int iterAnim ) override
        {
            grafik.setColour(Colours::white);
            grafik.drawRect(x, y + iterAnim/SPREZYSTOSC, szerokosc , wysokosc, 4);
        }
        
        virtual ~Przeszkoda() override {}
    };
    
    class Moneta : public Obiekt
    {
    public:
        Moneta(int xx1, int xx2, int xdelta)
        {
            x = xx1;
            y = xx2-xdelta;
            wysokosc = xdelta;
            szerokosc = xdelta;
        }
        
        void ruch(int ile)
        {
            const int KONIEC_EKRANU = 0;
            
            x -= PREDKOSC_OBIEKTOW;
            if ( x == KONIEC_EKRANU )
                szerokosc -= PREDKOSC_OBIEKTOW;
        }
        
        bool czyKolizja(Graphics& grafik)
        {
            return false;
        }
        
        void rysuj(Graphics& grafik)
        {
            grafik.setColour(Colours::white);
            grafik.drawEllipse(x, y, wysokosc, szerokosc, 5);
        }

        ~Moneta();

    };

    
private:
    //==============================================================================

    // Your private member variables go here...
    int iter_skok = 1;
    int iter_splas = 1;
    int ruchDziura = 0;
    bool jakiZnak = true;
    
    Gracz gracz;
    LookAndFeel_V4 otherLookAndFeel;
    TextButton button;
    std::vector<Przeszkoda> Przeszkody;
    std::queue<int> sprawdzator_Przeszkod;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
//
//
// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
