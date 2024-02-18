/*
@author- Kaevin Barta
@file- polygons.sln
@version- 0.3

The purpose of this assignment is to give me some experience using pointers to objects, 
and arrays of object pointers.
*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include "constants.h"
#include "spaceObject.h"

//Calculates if the ship has hit an astriod @param p1 - the location of first object @param p2 - the location of the secound object
bool objectsIntersect(Point p1, Point p2);
//get a random velocity for asteriods
Point getRandomLocation();
//get random location for the asteriods
Point getRandomVelocity();
//============================================================================
int main()
{
	//create graphics window
	sf::RenderWindow window(
		sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
		"Polygons!",
		sf::Style::Titlebar | sf::Style::Close //adds title bar and X button
	);

	window.setFramerateLimit(60);

	sf::Font font;
	if (!font.loadFromFile("C:\\Users\\HP\\Downloads\\Delicious_Handrawn\\Delicious.ttf"))
	{
		std::cerr << "Error loading font\n";
		return -1;
	}

	// Create a text object for the title
	sf::Text title("Polygons", font);
	title.setCharacterSize(48);
	title.setFillColor(sf::Color::White);

	// Center the title on the screen
	sf::FloatRect titleBounds = title.getLocalBounds();
	title.setOrigin(titleBounds.left + titleBounds.width / 2.0f,
		titleBounds.top + titleBounds.height / 2.0f);
	title.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 3.0f));

	//create SpaceObjects
	Point shipLocation;
	int twoAsteriods = 0;
	int asteriodRadius;
	Point shipVelocity;
	Point respawn;
	int photonTimer;
	Point photonLocation;
	float shipAngle;
	respawn.x = 200;
	respawn.y = 200;
	Point asteriodsLocation;
	int shipBlown = 0; // identifys if the ship has been blown, it can only blow once
	SpaceObject* ship = nullptr;
	ship = new SpaceObject;
	ship->setType(1);
	SpaceObject* asteriods[MAX_ASTEROIDS] = { nullptr };
	SpaceObject* photons[MAX_PHOTONS] = { nullptr };
	for (int x = 0; x < 5; x++) {
		Point location;
		Point velocity;
		location = getRandomLocation();
		velocity = getRandomVelocity();
		asteriods[x] = new SpaceObject;
		asteriods[x]->setLocation(location.x, location.y);
		asteriods[x]->setVelocity(velocity.x, velocity.y);
	}
		//TODO: create ship (step 3.2)
	srand(42);
		//game loop
	while (window.isOpen())
	{
		//handle user input (events and keyboard keys being pressed) ----
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				for (int x = 0; x < MAX_PHOTONS; x++)
					if (photons[x] == nullptr) {
						shipLocation = ship->getLocation();
						shipAngle = ship->getAngle();
						photons[x] = new SpaceObject;
						photons[x]->setType(2);
						photons[x]->setRadius(2);
						photons[x]->setLocation(shipLocation.x, shipLocation.y);
						photons[x]->setVelocity(shipVelocity.x, shipVelocity.y);
						photons[x]->setAngle(shipAngle);
						photons[x]->applyThrust(5);
						x = 10;//way out of the loop
					}
		}
		shipVelocity = ship->getVelocity();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			ship->changeAngle(-3);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			ship->changeAngle(3);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			ship->applyThrust(0.1);
  		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
	


		// update game objects ------------------------------------------
		for (int x = 0; x < MAX_ASTEROIDS; x++) {
			if (asteriods[x] != nullptr) {
				asteriods[x]->updatePosition();
				if (x % 2 == 1)//makes them spin diffent direction if they are odd in array
					asteriods[x]->changeAngle(-1);
				else
					asteriods[x]->changeAngle(1);
			}
		}

		for (int x = 0; x < MAX_PHOTONS; x++)
			if (photons[x] != nullptr) {
				photons[x]->updatePosition();
			}

	    ship->updatePosition(); 

		for (int x = 0; x < MAX_PHOTONS; x++) {
			if (photons[x] != nullptr) {
				photonTimer = photons[x]->getTimesDrawn();
				if (photonTimer > 100) {
					delete photons[x];
					photons[x] = nullptr;
				}
			}
		}
		// draw new frame ------------------------------------------------
		window.clear();

		for (int x = 0; x < MAX_ASTEROIDS; x++) {
			if (asteriods[x] != nullptr) {
				asteriods[x]->draw(window);
			}
		}
			ship->draw(window);
		
			
			for (int x = 0; x < MAX_ASTEROIDS; x++) {
				if (asteriods[x] != nullptr) {
					asteriodsLocation = asteriods[x]->getLocation();
					shipLocation = ship->getLocation();
					if (objectsIntersect(asteriodsLocation, shipLocation) && shipBlown == 0) {
						ship->explode();
						shipBlown += 1;
					}
				}
			}
			
			for (int y = 0; y < MAX_PHOTONS; y++) {
				if (photons[y] != nullptr) {
					photonLocation = photons[y]->getLocation();
					for (int x = 0; x < MAX_ASTEROIDS; x++)
					if (asteriods[x] != nullptr) {
						asteriodsLocation = asteriods[x]->getLocation();
						if (objectsIntersect(asteriodsLocation, photonLocation)) {
							asteriodRadius = asteriods[x]->getRadius();
							if (asteriodRadius > 6) {
								for (int z = 0; z < MAX_ASTEROIDS; z++) {
									if (asteriods[z] == nullptr && twoAsteriods < 2) {
										delete photons[y];
										photons[y] = nullptr;
										delete asteriods[x];
										asteriods[x] = nullptr;
										Point velocityNewObj;
										velocityNewObj = getRandomVelocity();
										asteriods[z] = new SpaceObject;
										asteriods[z]->setType(0);
										asteriods[z]->setRadius(asteriodRadius/2);
										asteriods[z]->setLocation(asteriodsLocation.x, asteriodsLocation.y);
										asteriods[z]->setVelocity(velocityNewObj.x, velocityNewObj.y);
										twoAsteriods += 1;
									}
								}
								twoAsteriods = 0;
							}
							else {
								delete photons[y];
								photons[y] = nullptr;
								delete asteriods[x];
								asteriods[x] = nullptr;
							}
					    }
					}
				}
			}

		for(int x = 0; x < MAX_PHOTONS; x++)
		if (photons[x] != nullptr) {
			photons[x]->draw(window);
		}

		if (shipLocation.x == respawn.x && shipLocation.y == respawn.y && shipBlown == 1) {
			shipBlown -= 1;
		}
		
		window.display();
		//reitterate the while loop
	}

	return 0;
}

Point getRandomLocation() {
	Point randNum;
	randNum.x = rand() % SCREEN_WIDTH + 1;
	randNum.y = rand() % SCREEN_HEIGHT + 1;
	return randNum;
}

Point getRandomVelocity() {
	Point randNum;
	int range = (.99 + .99 + .001) * 1000;
	randNum.x = (rand() % range) / 1000 - .99;
	randNum.y = (rand() % range) / 1000 - .99;
	return randNum;
}

bool objectsIntersect(Point p1, Point p2) {
	if ((p1.x - p2.x) < 30 && (p1.y - p2.y) < 30 && (p1.x - p2.x) > -30 && (p1.y - p2.y) > -30)
		return true;
	else
		return false;
}

