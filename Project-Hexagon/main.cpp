
#include <iostream>

#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <string>
#include <iomanip>
#include <unordered_set>
#include <functional>
#include <SFML/Graphics.hpp>

#include "window.hpp"
#include "time.hpp"
#include "camera.hpp"
#include "mouse.hpp"
#include "font.hpp"
#include "elementGUI.hpp"
#include "palette.hpp"
#include "map.hpp"



int main() {

	cam = new Camera();
	window->setView(cam->view);

	load_fonts();

	Map* mapa = new Map();
	palette = new Palette();

	sf::Text fps_counter_text("0", basic_font, 17);
	fps_counter_text.setPosition(cam->position - cam->view.getSize()/2.0f + sf::Vector2f(16,16));

	sf::Text cam_position_text("", basic_font, 17);
	cam_position_text.setPosition(cam->position - cam->view.getSize() / 2.0f + sf::Vector2f(16, 16) + sf::Vector2f(0, basic_font.getLineSpacing(17)));


	sf::Text tile_coords_text("", basic_font, 17);
	tile_coords_text.setPosition(cam->position - cam->view.getSize() / 2.0f + sf::Vector2f(16, 16) + sf::Vector2f(0, 2*basic_font.getLineSpacing(17)));

	while (window->isOpen()) {

		prevTime = currentTime;
		currentTime = timeClock.getElapsedTime();

		// camera movements
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			float dt = currentTime.asSeconds() - prevTime.asSeconds();
			float moveSpeed = 300.0f * dt;
			cam->move(0.0f, -moveSpeed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			float dt = currentTime.asSeconds() - prevTime.asSeconds();
			float moveSpeed = 300.0f * dt;
			cam->move(-moveSpeed, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			float dt = currentTime.asSeconds() - prevTime.asSeconds();
			float moveSpeed = 300.0f * dt;
			cam->move(0.0f, moveSpeed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			float dt = currentTime.asSeconds() - prevTime.asSeconds();
			float moveSpeed = 300.0f * dt;
			cam->move(moveSpeed, 0.0f);
		}

		cam->update();
		window->setView(cam->view);

		mousePosition = sf::Mouse::getPosition(*window);
		worldMousePosition = window->mapPixelToCoords(mousePosition);

		palette->update();
		// update GUI elements
		// ..
		// ..
		
		// cursor hovering
		ElementGUI_hovered = nullptr;
		palette->cursorHover();
		mapa->cursorHover();

		float FPS = 1.0f / FPSClock.restart().asSeconds();
		if (FPSClockUpdate.getElapsedTime().asSeconds() > 0.5f) {

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << FPS << " FPS";
			window->setTitle("Project Hexagon - " + ss.str());
			fps_counter_text.setString(ss.str());
			FPSClockUpdate.restart();
		}

		fps_counter_text.setPosition(cam->position - cam->view.getSize() / 2.0f + sf::Vector2f(16, 16));

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

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				palette->handleEvent(event);
			}

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				palette->handleEvent(event);
				ElementGUI_pressed = nullptr;
			}

			if (event.type == sf::Event::MouseMoved) {
				palette->handleEvent(event);

			}

			
		}

		// update
		sf::CircleShape c(2);
		c.setOrigin(2, 2);
		c.setFillColor(sf::Color::White);
		c.setPosition(mapa->getTile(0, 0, 0)->hexagon.getPoint(i));

		std::ostringstream os;
		os << std::fixed << std::setprecision(2) << cam->position.x << " " << cam->position.y;
		cam_position_text.setString(os.str());
		cam_position_text.setPosition(cam->position - cam->view.getSize() / 2.0f + sf::Vector2f(16, 16) + sf::Vector2f(0, basic_font.getLineSpacing(17)));

		if (mapa->selected_tile != nullptr)
			tile_coords_text.setString(
				"tile: " +
				std::to_string(mapa->selected_tile->coords.x) + " " +
				std::to_string(mapa->selected_tile->coords.y) + " " +
				std::to_string(mapa->selected_tile->coords.z));
		else
			tile_coords_text.setString("none");
		tile_coords_text.setPosition(cam->position - cam->view.getSize() / 2.0f + sf::Vector2f(16, 16) + sf::Vector2f(0, 2 * basic_font.getLineSpacing(17)));

		// render
		window->clear();
		mapa->draw();
		window->draw(c);
		palette->draw();
		window->draw(fps_counter_text);
		window->draw(cam_position_text);
		window->draw(tile_coords_text);
		window->display();
	}
	return 0;
}
