
#include <iostream>

#include <sstream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <string>
#include <iomanip>
#include <unordered_set>
#include <SFML/Graphics.hpp>

#include "window.hpp"
#include "time.hpp"
#include "camera.hpp"
#include "mouse.hpp"
#include "font.hpp"
#include "map.hpp"


int main() {

	cam = new Camera();
	window->setView(cam->view);

	load_fonts();

	Map* mapa = new Map();

	sf::Text fps_counter_text("", basic_font, 17);
	fps_counter_text.setPosition(cam->position - cam->view.getSize()/2.0f + sf::Vector2f(16,16));

	sf::Text tile_coords_text("", basic_font, 17);
	tile_coords_text.setPosition(cam->position - cam->view.getSize() / 2.0f + sf::Vector2f(16, 16) + sf::Vector2f(0, basic_font.getLineSpacing(17)));

	while (window->isOpen()) {

		prevTime = currentTime;
		currentTime = timeClock.getElapsedTime();

		cam->update();
		window->setView(cam->view);

		mousePosition = sf::Mouse::getPosition(*window);
		worldMousePosition = window->mapPixelToCoords(mousePosition);

		// update GUI elements
		// ..
		// ..
		
		// cursor hovering
		mapa->cursorHover();

		float FPS = 1.0f / FPSClock.restart().asSeconds();
		if (FPSClockUpdate.getElapsedTime().asSeconds() > 0.5f) {

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << FPS << " FPS";
			window->setTitle("Project Hexagon - " + ss.str());
			fps_counter_text.setString(ss.str());
			FPSClockUpdate.restart();
		}

		static int i = 0;

		// handle events
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				i += 1;
				i %= 6;
			}
				
		}

		// update
		sf::CircleShape c(2);
		c.setOrigin(2, 2);
		c.setFillColor(sf::Color::White);
		c.setPosition(mapa->getTile(0, 0, 0)->hexagon.getPoint(i));

		if (mapa->selected_tile != nullptr)
			tile_coords_text.setString(
				"tile: " +
				std::to_string(mapa->selected_tile->coords.x) + " " + 
				std::to_string(mapa->selected_tile->coords.y) + " " + 
				std::to_string(mapa->selected_tile->coords.z));
		else
			tile_coords_text.setString("none");

		// render
		sf::View v;
		v = window->getView();
		v.setCenter(0,0);
		window->setView(v);
		window->clear();
		mapa->draw();
		window->draw(c);
		window->draw(fps_counter_text);
		window->draw(tile_coords_text);
		window->display();
	}
	return 0;
}
