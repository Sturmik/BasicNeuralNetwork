#include "ExampleProjects.h"

#include <SFML/Graphics.hpp>

#include "NeuralNetwork.h"
#include "Utility.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													XOR PROBLEM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void XORProblemSolvingExample()
{
	const int32_t WindowWidth = 800;
	const int32_t WindowHeight = 800;

	srand(time(NULL));

	// Example of XOR solving network

	// Create network
	NeuralNetwork XORNetwork(2, 100, 1, 0.1, -0.3, 0.3);

	// Setup training data
	std::vector<std::vector<float>> InputArray = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	std::vector<std::vector<float>> Targets = { {0}, {1}, {1}, {0} };

	// Render
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "XORProblemSolvingExample");

	while (Window.isOpen())
	{
		Window.clear();

		// Train network
		for (int I = 0; I < 1000; ++I)
		{
			int RandomIndex = rand() % InputArray.size();
			XORNetwork.Train(InputArray[RandomIndex], Targets[RandomIndex]);
		}

		int Resolution = 10;
		int Columns = WindowWidth / Resolution;
		int Rows = WindowHeight / Resolution;

		for (int I = 0; I < Columns; ++I)
		{
			for (int J = 0; J < Rows; ++J)
			{
				// Calculate X and Y position
				float X = (float)I / Columns;
				float Y = (float)J / Rows;
				// Form input coordinates
				std::vector<float> InputsCoordinates = { X, Y };
				// Get prediction
				float Prediction = XORNetwork.Predict(InputsCoordinates)[0];

				// Set up the rectangle
				sf::RectangleShape Rectangle;
				// Set the size of the rectangle
				Rectangle.setSize(sf::Vector2f(Resolution, Resolution));
				// Set position
				Rectangle.setPosition(J * Resolution, I * Resolution);
				// Set the BackgroundColor of the rectangle based on prediction
				float FillColor = 255 * Prediction;
				Rectangle.setFillColor(sf::Color(FillColor, FillColor, FillColor));
				// Draw rectangle
				Window.draw(Rectangle);
			}
		}

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				XORNetwork = NeuralNetwork(2, 4, 1, 0.1, -0.3, 0.3);
			}
		}

		Window.display();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													COLOR PREDICTOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sf::Color GetRandomColor()
{
	return sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

// Returns suitable foreground BackgroundColor of text for the given background with RGB BackgroundColor scheme
sf::Color ColorPredictor(NeuralNetwork& ColorPredictorNetwork, float R, float G, float B)
{
	// Prepare inputs
	std::vector<float> Inputs = { R / 255, G / 255, B / 255 };
	// Get prediction from network
	std::vector<float> Outputs = ColorPredictorNetwork.Predict(Inputs);
	std::cout << "Black color percentage: " << Outputs[0] << std::endl;
	std::cout << "White color percentage: " << Outputs[1] << std::endl << std::endl;
	// We consider that outputs correlate with colors in next way:
	// [0] - Black
	// [1] - White
	if (Outputs[0] > Outputs[1])
	{
		return sf::Color::Black;
	}
	else
	{
		return sf::Color::White;
	}
}

float GetColorLuminance(sf::Color Color)
{
	// Convert RGB to HSL
	float R = Color.r / 255.0f;
	float G = Color.g / 255.0f;
	float B = Color.b / 255.0f;

	float MaxVal = std::max({ R, G, B });
	float MinVal = std::min({ R, G, B });

	return (MaxVal + MinVal) / 2.0f;
}

std::vector<float> TrainColor(sf::Color BackgroundColor)
{
	float Luminance = GetColorLuminance(BackgroundColor);

	if (Luminance > 0.5)
	{
		// Black BackgroundColor
		return std::vector<float>() = { 1.0f, 0.0f };
	}
	else
	{
		// White BackgroundColor
		return std::vector<float>() = { 0.0f, 1.0f };
	}
}

void ColorPredictorExample()
{
	const int32_t WindowWidth = 800;
	const int32_t WindowHeight = 400;

	srand(time(NULL));

	// Example of BackgroundColor predictor solving network

	// Create network
	NeuralNetwork ColorPredictorNetwork(3, 3, 2, 0.1, -1, 1);

	// Train network
	for (int I = 0; I < 10000; ++I)
	{
		sf::Color RandomColor = GetRandomColor();
		std::vector<float> Inputs = { (float)RandomColor.r, (float)RandomColor.g,(float)RandomColor.b };
		ColorPredictorNetwork.Train(Inputs, TrainColor(RandomColor));
	}

	// Render window and its BackgroundColor
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "ColorPredictorExample");
	sf::Color WindowColor = GetRandomColor();

	// Load a LabelFont
	sf::Font LabelFont;
	if (!LabelFont.loadFromFile("Resources\\Fonts\\Montserrat-Light.ttf"))
	{
		std::cout << "Font loading failed!" << std::endl;
		return;
	}

	// Create a TestText object
	sf::Text TestText;

	// Set the LabelFont
	TestText.setFont(LabelFont);

	// Set the string to display
	TestText.setString("Test Color!");

	// Set the character size
	TestText.setCharacterSize(34);

	// Set origin
	sf::FloatRect FloatRectTestText = TestText.getLocalBounds();
	TestText.setOrigin(FloatRectTestText.width / 2, FloatRectTestText.height / 2);

	// Define the MiddleLine properties
	sf::VertexArray MiddleLine(sf::Lines, 2);
	// Starting point (middle top)
	MiddleLine[0].position = sf::Vector2f(Window.getSize().x / 2, 0);
	// Ending point (middle bottom)
	MiddleLine[1].position = sf::Vector2f(Window.getSize().x / 2, Window.getSize().y);
	MiddleLine[0].BackgroundColor = sf::Color::Black;
	MiddleLine[1].BackgroundColor = sf::Color::Black;

	// Create a ColorPickerCircle shape BackgroundColor picker
	sf::CircleShape ColorPickerCircle(25.0f);

	// Set the origin to the center of the ColorPickerCircle
	ColorPickerCircle.setOrigin(ColorPickerCircle.getRadius(), ColorPickerCircle.getRadius());

	// Position the ColorPickerCircle in the center of the Window
	ColorPickerCircle.setPosition(Window.getSize().x / 2.0f, Window.getSize().y / 2.0f);

	// Set the fill BackgroundColor
	ColorPickerCircle.setFillColor(sf::Color::White);

	// Color chosen by network
	sf::Color NetworkChosenColor = ColorPredictor(ColorPredictorNetwork, (float)WindowColor.r, (float)WindowColor.g, (float)WindowColor.b);

	while (Window.isOpen())
	{
		Window.clear(WindowColor);

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// Send target for network training

				// Prepare inputs as window BackgroundColor
				std::vector<float> Inputs = { (float)WindowColor.r, (float)WindowColor.g,(float)WindowColor.b };
				// Black BackgroundColor for text is better
				if (sf::Mouse::getPosition(Window).x < WindowWidth / 2)
				{
					// Create vector for targets, which will train neural network
					std::vector<float> Targets = { 1.0f, 0.0f };
					ColorPredictorNetwork.Train(Inputs, Targets);
				}
				// White BackgroundColor for text is better
				else
				{
					// Create vector for targets, which will train neural network
					std::vector<float> Targets = { 0.0f, 1.0f };
					ColorPredictorNetwork.Train(Inputs, Targets);
				}

				// Update background BackgroundColor
				WindowColor = GetRandomColor();
				NetworkChosenColor = ColorPredictor(ColorPredictorNetwork, WindowColor.r, WindowColor.g, WindowColor.b);
			}
			if (event.type == sf::Event::KeyPressed)
			{
				// Update background BackgroundColor
				WindowColor = GetRandomColor();
				NetworkChosenColor = ColorPredictor(ColorPredictorNetwork, WindowColor.r, WindowColor.g, WindowColor.b);
			}
		}

		// Setup text on the left side
		TestText.setPosition(WindowWidth / 4, WindowHeight / 2);
		TestText.setFillColor(sf::Color::Black);
		Window.draw(TestText);

		// Setup text on the right side
		TestText.setPosition(WindowWidth - (WindowWidth / 4), WindowHeight / 2);
		TestText.setFillColor(sf::Color::White);
		Window.draw(TestText);

		// Draw the middle line
		Window.draw(MiddleLine);

		// Draw PaintCircleShape upon BackgroundColor, which was chosen by network
		if (NetworkChosenColor == sf::Color::Black)
		{
			ColorPickerCircle.setPosition(WindowWidth / 4, WindowHeight / 4);
		}
		else if (NetworkChosenColor == sf::Color::White)
		{
			ColorPickerCircle.setPosition(WindowWidth - (WindowWidth / 4), WindowHeight / 4);
		}
		Window.draw(ColorPickerCircle);

		Window.display();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													NUMBER PREDICTOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string GetLastElementOfPath(const std::string& Path)
{
	size_t Found = Path.find_last_of("/\\"); // Find the last occurrence of a Path separator
	if (Found != std::string::npos && Found != Path.length() - 1)
	{
		return Path.substr(Found + 1); // Extract the substring after the last separator
	}
	else
	{
		// If the Path ends with a separator or no separator is Found, return the original Path
		return Path;
	}
}

std::pair<int, std::string> GetRandomFile(const std::string& FolderPath)
{
	std::string Command = "dir /b \"" + FolderPath + "\"";
	FILE* Pipe = _popen(Command.c_str(), "r");
	if (!Pipe) {
		std::cerr << "Error: Unable to open directory." << std::endl;
		return std::pair<int, std::string>();
	}

	std::vector<std::string> Files;
	char Buffer[128];
	while (fgets(Buffer, 128, Pipe) != nullptr) {
		std::string FileName = Buffer;
		// Remove trailing whitespace and newline
		FileName.erase(FileName.find_last_not_of(" \n\r\t") + 1);
		std::replace(FileName.begin(), FileName.end(), '/', '\\');
		Files.push_back(FileName);
	}

	_pclose(Pipe);

	if (Files.empty()) {
		std::cerr << "No files found in the specified folder." << std::endl;
		return std::pair<int, std::string>();
	}

	int RandomIndex = rand() % Files.size();
	return std::pair<int, std::string>(std::stoi(GetLastElementOfPath(FolderPath)), FolderPath + "\\" + Files[RandomIndex]);
}

std::vector<float> GetPixelLuminanceArrayFromImage(sf::Image TargetImage)
{
	std::vector<float> PixelLuminanceArray;
	for (int Y = 0; Y < TargetImage.getSize().y; ++Y)
	{
		for (int X = 0; X < TargetImage.getSize().x; ++X)
		{
			PixelLuminanceArray.emplace_back(GetColorLuminance(TargetImage.getPixel(X, Y)));
		}
	}
	return PixelLuminanceArray;
}

void PrintNumberPredictionResult(std::vector<float> GuessArray)
{
	// Output guess results
	if (GuessArray.size() == 10)
	{
		std::cout << "Guess Number Percentage" << std::endl << std::endl;
		int BestGuessIndex = 0;
		for (int I = 0; I < GuessArray.size(); ++I)
		{
			std::cout << I << " = " << GuessArray[I] << std::endl;
			if (I != BestGuessIndex)
			{
				if (GuessArray[I] > GuessArray[BestGuessIndex])
				{
					BestGuessIndex = I;
				}
			}
		}
		std::cout << "\nNumber is " << BestGuessIndex << std::endl;
		std::cout << std::endl;
	}
	else
	{
		std::cout << "\nImage must have size 28x28 in order to be analyzed!" << std::endl << std::endl;
	}
}

sf::Image ResizeImage(const sf::Image& OriginalImage, unsigned int NewWidth, unsigned int NewHeight)
{
	// Create a new image with the desired size
	sf::Image ResizedImage;
	ResizedImage.create(NewWidth, NewHeight);

	// Create a Sprite and set the Texture from the original image
	sf::Texture Texture;
	Texture.loadFromImage(OriginalImage);
	sf::Sprite Sprite(Texture);

	// Scale the Sprite to fit the new size
	float ScaleX = static_cast<float>(NewWidth) / Sprite.getGlobalBounds().width;
	float ScaleY = static_cast<float>(NewHeight) / Sprite.getGlobalBounds().height;
	Sprite.setScale(ScaleX, ScaleY);

	// Render the scaled Sprite to the resized image
	sf::RenderTexture RenderTexture;
	RenderTexture.create(NewWidth, NewHeight);
	RenderTexture.clear();
	RenderTexture.draw(Sprite);
	RenderTexture.display();
	ResizedImage = RenderTexture.getTexture().copyToImage();

	return ResizedImage;
}

void NumberRecognitionExample(bool CanDrawCheck)
{
	// Set window size
	const int32_t WindowWidth = 392;
	const int32_t WindowHeight = 392;

	srand(time(NULL));

	// Path to training data, which contains numbers
	const std::vector<std::string> PathsToTheTrainingNumberFoldersArray = {
	"Resources\\Data\\Training\\Numbers\\0",
	"Resources\\Data\\Training\\Numbers\\1",
	"Resources\\Data\\Training\\Numbers\\2",
	"Resources\\Data\\Training\\Numbers\\3",
	"Resources\\Data\\Training\\Numbers\\4",
	"Resources\\Data\\Training\\Numbers\\5",
	"Resources\\Data\\Training\\Numbers\\6",
	"Resources\\Data\\Training\\Numbers\\7",
	"Resources\\Data\\Training\\Numbers\\8",
	"Resources\\Data\\Training\\Numbers\\9"
	};

	// Create network
	// There are 784 pixels in each picture
	// There are 10 possible numbers on them
	NeuralNetwork NumberPredictorNetwork(784, 235, 10, 0.1, -0.3, 0.3);

	// Train network
	for (int I = 0; I < 2000; ++I)
	{
		// Get random file 
		// Int - number
		// string - path to the file of Image with given number
		std::pair<int, std::string> RandomTestFile = GetRandomFile(PathsToTheTrainingNumberFoldersArray[rand() % PathsToTheTrainingNumberFoldersArray.size()]);

		// Get Image in order to obtain pixel colors
		sf::Image Image;
		Image.loadFromFile(RandomTestFile.second);

		// Form input
		std::vector<float> Inputs = GetPixelLuminanceArrayFromImage(Image);

		// Form target
		std::vector<float> Targets(10);
		// Set number target output with value of 1.0, so network will train to recognize this number
		Targets[RandomTestFile.first] = 1.0f;

		// Train
		NumberPredictorNetwork.Train(Inputs, Targets);
	}

	// Render window and its BackgroundColor
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "ColorPredictorExample");

	// Sprite to visualise
	sf::Texture CurrentNumberTexture;
	sf::Sprite CurrentNumberSprite;

	// Variable to track if the left mouse button is pressed
	bool IsDrawing = false;
	// Vector to store PaintCircleShape shapes, which are used for painting
	std::vector<sf::CircleShape> PaintCircleShapeArray;

	while (Window.isOpen())
	{
		Window.clear();

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
			// User can draw own number
			if (CanDrawCheck)
			{
				if (event.type == sf::Event::MouseButtonPressed)
				{
					// Start drawing
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						IsDrawing = true;
					}
					// Guess
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						// Create a render texture to capture the drawn content
						sf::RenderTexture CaptureTexture;
						CaptureTexture.create(WindowWidth, WindowHeight);
						CaptureTexture.clear();

						// Draw the current content onto the render texture
						for (const auto& PaintCircleShape : PaintCircleShapeArray)
						{
							CaptureTexture.draw(PaintCircleShape);
						}

						CaptureTexture.display();
						sf::Texture Texture = CaptureTexture.getTexture();

						// Create an image from the texture
						sf::Image Image = Texture.copyToImage();

						// Get resized image
						sf::Image ResizedImage = ResizeImage(Image, 28, 28);

						// Guess what number this image represents
						std::vector<float> GuessArray = NumberPredictorNetwork.Predict(GetPixelLuminanceArrayFromImage(ResizedImage));

						PrintNumberPredictionResult(GuessArray);
					}
				}
				if (event.type == sf::Event::MouseButtonReleased)
				{
					// Stop drawing
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						IsDrawing = false;
					}
				}
				if (IsDrawing && event.type == sf::Event::MouseMoved)
				{
					// Draw continuously while the left mouse button is held down
					sf::CircleShape PaintCircleShape;
					PaintCircleShape.setFillColor(sf::Color::White);
					PaintCircleShape.setRadius(10.0f);
					// Set PaintCircleShape position at the center of the mouse pointer
					PaintCircleShape.setOrigin(PaintCircleShape.getRadius(), PaintCircleShape.getRadius());
					PaintCircleShape.setPosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));

					PaintCircleShapeArray.push_back(PaintCircleShape);
				}
				if (event.type == sf::Event::KeyPressed)
				{
					// Clear screen
					if (event.mouseButton.button == sf::Keyboard::Space)
					{
						PaintCircleShapeArray.clear();
					}
				}
			}
			// Program checks
			else
			{
				if (event.type == sf::Event::MouseButtonPressed)
				{
					// Path to training data, which contains numbers
					const std::vector<std::string> PathsToTheTestingNumberFoldersArray = {
					"Resources\\Data\\Test\\Numbers\\0",
					"Resources\\Data\\Test\\Numbers\\1",
					"Resources\\Data\\Test\\Numbers\\2",
					"Resources\\Data\\Test\\Numbers\\3",
					"Resources\\Data\\Test\\Numbers\\4",
					"Resources\\Data\\Test\\Numbers\\5",
					"Resources\\Data\\Test\\Numbers\\6",
					"Resources\\Data\\Test\\Numbers\\7",
					"Resources\\Data\\Test\\Numbers\\8",
					"Resources\\Data\\Test\\Numbers\\9"
					};

					// Get random file 
					std::pair<int, std::string> RandomTestFile = GetRandomFile(PathsToTheTrainingNumberFoldersArray[rand() % PathsToTheTrainingNumberFoldersArray.size()]);

					// Load Texture from file
					CurrentNumberTexture.loadFromFile(RandomTestFile.second);

					// Create Sprite and set its Texture
					CurrentNumberSprite.setTexture(CurrentNumberTexture);

					// Upscale Image
					CurrentNumberSprite.setScale(WindowWidth / CurrentNumberTexture.getSize().x,
						WindowHeight / CurrentNumberTexture.getSize().y);

					// Get Image in order to obtain pixel colors
					sf::Image Image;
					Image.loadFromFile(RandomTestFile.second);

					// Guess what is this number
					std::vector<float> GuessArray = NumberPredictorNetwork.Predict(GetPixelLuminanceArrayFromImage(Image));

					PrintNumberPredictionResult(GuessArray);
				}
			}
		}

		// Draw current number
		Window.draw(CurrentNumberSprite);

		// Draw paint PaintCircleShape shapes
		for (int I = 0; I < PaintCircleShapeArray.size(); ++I)
		{
			Window.draw(PaintCircleShapeArray[I]);
		}

		Window.display();
	}
}
