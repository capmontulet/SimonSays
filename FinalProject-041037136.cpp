#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include <vector>
#include <omp.h>


using namespace std;

sf::SoundBuffer buffer1, buffer2, buffer3, buffer4, buffer5;
sf::Sound sound1, sound2, sound3, sound4, sound5;
sf::Clock clock1;
sf::Clock clock2;
int timeSpeed = 500;
bool gameOverBool;

void playSound(int num);
void gameOver(int count, sf::Clock clock2);

int main(int argc, const char* argv[]) {
    sf::RenderWindow window{ {800, 600},"Simon Says" };
    tgui::Gui gui{ window };
    gui.loadWidgetsFromFile("form.txt");

    // instantiating sequence and color vectors
    std::vector<std::string> colors = { "red", "green", "yellow", "blue" };
    std::vector<int> sequence;
    int current_index = 0;
    int count = 0;

    //instantiating sounds
    buffer1.loadFromFile("tone1.wav");
    buffer2.loadFromFile("tone2.wav");
    buffer3.loadFromFile("tone3.wav");
    buffer4.loadFromFile("tone4.wav");
    buffer5.loadFromFile("tone5.wav");

    // setting sounds to buffer
    sound1.setBuffer(buffer1);
    sound2.setBuffer(buffer2);
    sound3.setBuffer(buffer3);
    sound4.setBuffer(buffer4);
    sound5.setBuffer(buffer5);

    // start New Game button callback function
    gui.get<tgui::Button>("startGameButton")->onPress([&]() {
        // clears previous sequence if this is pressed after a game
        sequence.clear();
    current_index = 0;
    timeSpeed = 500;
    gameOverBool = false;
 
    // randomly generate first color in the sequence
    std::srand(std::time(nullptr));
    sequence.push_back(std::rand() % colors.size());

    // shows the first color in the sequence
    tgui::Button::Ptr button = gui.get<tgui::Button>(colors[sequence[0]] + "Button");
    tgui::Color originalColor = button->getRenderer()->getBackgroundColor();

    // highlighting the color of the first button white
    button->getRenderer()->setBackgroundColor(tgui::Color::White);
    gui.draw();
    window.display();
    sf::sleep(sf::milliseconds(500));

    // setting back to original color
    button->getRenderer()->setBackgroundColor(originalColor);

    playSound(sequence[0]);
        });


    // color button callback function
    for (int i = 0; i < colors.size(); ++i) {

        clock2.restart();

        gui.get<tgui::Button>(colors[i] + "Button")->onPress([&, i]() {
            // check if user pressed the correct color in the sequence
            if (i == sequence[current_index]) {
                // if user pressed the correct color index is increased to allow for another colour
                ++current_index;

                playSound(i);

                // check if user has pressed all colors in the sequence
                if (current_index == sequence.size()) {
                    // user has pressed all colors in the sequence, add one more color to the sequence
                    sequence.push_back(std::rand() % colors.size());
                    current_index = 0;

                    // show the entire sequence
                    for (int j = 0; j < sequence.size(); ++j) {
                        tgui::Button::Ptr button = gui.get<tgui::Button>(colors[sequence[j]] + "Button");
                        tgui::Color originalColor = button->getRenderer()->getBackgroundColor();

                        // plays sound of the button in sequence
                        playSound(sequence[j]);

                        // shows button as white
                        button->getRenderer()->setBackgroundColor(tgui::Color::White);
                        gui.draw();
                        window.display();

                        // wait for the length of timeSpeed
                        clock1.restart();
                        while (clock1.getElapsedTime().asMilliseconds() < timeSpeed) {}

                        // set button color back
                        button->getRenderer()->setBackgroundColor(originalColor);
                        gui.draw();
                        window.display();

                        // wait for the length of timeSpeed
                        clock1.restart();
                        while (clock1.getElapsedTime().asMilliseconds() < timeSpeed) {}
                        
                        clock2.restart();
                    }
                    // count increased and timeSpeed is decreased to speed game up
                    count++;
                    timeSpeed -= 30;
                }
            }
            else {
                // user pressed the wrong color, game over
                sound5.play();
                cout << "Sorry, wrong color. You made it to round " << count << " of the game. " << endl;
                cout << "Press 'Start Game' to start another game" << endl;
            }
            });
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                clock2.restart();
            }
            gui.handleEvent(event);
        }

        window.clear();
        gui.draw();
        window.display();
        
        if (gameOverBool == false) {
            if (clock2.getElapsedTime().asSeconds() > 5) {
                gameOver(count, clock2);
                gameOverBool = true;
            }
        }
    }

    return 0;

}


// plays the sounds
void playSound(int num) {

    switch (num) {
    case 0:
        sound1.play();
        break;
    case 1:
        sound2.play();
        break;
    case 2:
        sound3.play();
        break;
    case 3:
        sound4.play();
        break;
    }
}

void gameOver(int count, sf::Clock clock2) {
    sound5.play();
    cout << "Sorry, wrong color. You made it to round " << count << " of the game. " << endl;
    cout << "Press 'Start Game' to start another game" << endl;
    clock2.restart();
}