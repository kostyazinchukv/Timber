//Timber game
#include<sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	// Create a video mode object
	VideoMode vm(1440, 900);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");

	
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	//tree texture and sprites
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(570, 0);

	//bee texture and sprites
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	int beeSpeed = 0;

	//clouds texture and sprites
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);

	spriteCloud2.setTexture(textureCloud);
	spriteCloud2.setPosition(0, 125);

	spriteCloud3.setTexture(textureCloud);
	spriteCloud3.setPosition(0,250);

	bool isCloud1Active = false;
	bool isCloud2Active = false;
	bool isCloud3Active = false;

	int cloud1Speed = 0;
	int cloud2Speed = 0;
	int cloud3Speed = 0;

	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400.0f;
	float timeBarHeight = 80.0f;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition(1440 / 2 - timeBarStartWidth/2, 800);
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//game vitals boolean
	bool paused = true;
	int score = 0;

	//Text box
	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press enter to start");
	scoreText.setString("Score = ");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color:: White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1440 / 2.0f, 900 / 2.0f);
	scoreText.setPosition(20, 20);

	while (window.isOpen())
	{
		//exit game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//pausing game
		if (Keyboard::isKeyPressed(Keyboard::Enter))
		{
			paused = false;
			score = 0;
			timeRemaining = 6.0f;
		}


		// Clear everything from the last frame
		window.clear();
		if (!paused) {
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of time");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1440 / 2.0f, 900 / 2.0f);
			}
			//Manage bee moving
			if (!beeActive)
			{
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				srand((int)time(0) * 10);
				int height = (rand() % 400) + 400;
				spriteBee.setPosition(static_cast<float>(1500), static_cast<float>(height));
				beeActive = true;
			}
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			//Manage the clouds moving
			if (!isCloud1Active)
			{
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				srand((int)time(0) * 10);
				int height = rand() % 125;
				spriteCloud1.setPosition(static_cast<float>(-150), static_cast<float>(height));
				isCloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1550)
				{
					isCloud1Active = false;
				}
			}
			if (!isCloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				int height = rand() % 125;
				spriteCloud2.setPosition(static_cast<float>(-150), static_cast<float>(height));
				isCloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1550)
				{
					isCloud2Active = false;
				}
			}
			if (!isCloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				int height = rand() % 250;
				spriteCloud3.setPosition(static_cast<float>(-150), static_cast<float>(height));
				isCloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1550)
				{
					isCloud3Active = false;
				}
			}
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
		}
		// Draw game scene here
		window.draw(spriteBackground);
		window.draw(spriteTree);
		window.draw(scoreText);
		if (paused) {
			window.draw(messageText);
		}
		window.draw(timeBar);
		window.draw(spriteBee);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);




		// Show everything just drew
		window.display();


	}

	return 0;
}