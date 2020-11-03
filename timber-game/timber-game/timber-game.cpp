//Timber game
#include<sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
void updateBranches(int seed);
const int numBranch = 6;
Sprite branches[numBranch];
enum class side{LEFT, RIGHT, NONE};
side branchPositions[numBranch];

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
	spriteTree.setPosition(675, 0);

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

	//branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < numBranch; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(540, 600);
	side playerSide = side::LEFT;

	//tombstone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(540, 820);
	side sideRIP = side::NONE;

	//axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(650, 700);

	const int AXE_POSITION_LEFT = 650;
	const int AXE_POSITION_RIGHT = 845;

	//piece of tree choped every time
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(675, 620);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer timeBuffer;
	timeBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(timeBuffer);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
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
			//clear branches
			for (int i = 0; i < numBranch; i++)
			{
				branchPositions[i] = side::NONE;
			}
			spriteRIP.setPosition(560, 2000);
			spritePlayer.setPosition(540, 600);
			acceptInput = true;

		}
		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += static_cast<float>((2 / score) + .15);
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(940, spritePlayer.getPosition().y);
				updateBranches(score);
				spriteLog.setPosition(675, 620);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += static_cast<float>((2 / score) + .15);
				if (timeRemaining > 6.0f) { timeRemaining = 6.0f; }
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(540, spritePlayer.getPosition().y);
				updateBranches(score);
				spriteLog.setPosition(675, 620);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}
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
				outOfTime.play();
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

			for (int i = 0; i < numBranch; i++)
			{
				int height = i * 125;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(455, static_cast<float>(height));
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1145, static_cast<float>(height));
					branches[i].setRotation(0);
				}
				else 
				{
					branches[i].setPosition(3000, 3000);
				}
			}
			// Handle a flying log				
			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the insect reached the right hand edge of the screen?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 1700)
				{
					// Set it up ready to be a whole new cloud next frame
					logActive = false;
					spriteLog.setPosition(675, 620);
				}
			}

			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				sideRIP = playerSide;

				// Draw the gravestone
				if (sideRIP == side::LEFT) 
				{
					spriteRIP.setPosition(520, 630);
				}
				else if (sideRIP == side::RIGHT)
				{
					spriteRIP.setPosition(960, 630);
				}
				spriteAxe.setPosition(2000, 2000);
				spriteLog.setPosition(675, 620);

				// hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1440 / 2.0f,
					900 / 2.0f);

				death.play();


			}
		}
		// Draw game scene here
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteRIP);
		window.draw(spriteLog);
		window.draw(spriteAxe);
		window.draw(spriteBee);
		window.draw(scoreText);
		if (paused) {
			window.draw(messageText);
		}
		window.draw(timeBar);

		for (int i = 0; i < numBranch; i++) {
			window.draw(branches[i]);
		}




		// Show everything just drew
		window.display();


	}

	return 0;
}

void updateBranches(int seed)
{
	for (int i = numBranch - 1; i > 0; i--)
	{
		branchPositions[i] = branchPositions[i - 1];
	}
	srand(static_cast<int>(time(0)) + seed);
	int rand_side = rand() % 5;
	switch (rand_side)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
