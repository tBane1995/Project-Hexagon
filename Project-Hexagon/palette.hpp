#ifndef palette_hpp
#define palette_hpp

enum class ButtonState { Idle, Hover, Pressed };


class Palette : public ElementGUI {
public:
	sf::Vector2f bar_size;
	sf::Vector2f size;
	sf::Vector2f position;

	sf::RectangleShape bar_rect;	// top bar rect
	sf::RectangleShape rect;		// bottom big rect

	short margin = 8;

	sf::Vector2f offset;
	bool is_moved;


	Palette() : ElementGUI() {

		sf::Vector2i tiles(2, 2);

		bar_size = sf::Vector2f(tiles.x * 64 + (tiles.x + 1) * margin, 24);
		size = sf::Vector2f(tiles.x * 64 + (tiles.x + 1) * margin, tiles.y * 64 + (tiles.y + 1) * margin);

		bar_rect = sf::RectangleShape(bar_size);
		bar_rect.setFillColor(sf::Color(32, 32, 32));
		bar_rect.setOutlineThickness(2.0f);
		bar_rect.setOutlineColor(sf::Color(32, 32, 32));

		rect = sf::RectangleShape(size);
		rect.setFillColor(sf::Color(64, 64, 64));
		rect.setOutlineThickness(2.0f);
		rect.setOutlineColor(sf::Color(32, 32, 32));

		is_moved = false;

		setPosition(sf::Vector2f(cam->view.getSize().x / 2 - size.x - 32, -cam->view.getSize().y / 2 + 32));
	}


	~Palette() {}

	void setPosition(sf::Vector2f position) {
		this->position = position;

		bar_rect.setPosition(this->position + cam->position);
		rect.setPosition(this->position + cam->position + sf::Vector2f(0, bar_size.y));

	}

	void cursorHover() {
		if (bar_rect.getGlobalBounds().contains(worldMousePosition)) {
			ElementGUI_hovered = this;
			return;
		}

		if (rect.getGlobalBounds().contains(worldMousePosition)) {
			ElementGUI_hovered = this;
		}

	}

	void handleEvent(sf::Event& event)  {
		
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

			std::cout << worldMousePosition.x << " " << worldMousePosition.y << "\n";
			
			// teraz działa
			if (bar_rect.getGlobalBounds().contains(worldMousePosition)) {
				is_moved = true;
				offset = worldMousePosition - cam->position - position;
				ElementGUI_pressed = this;
			}
			
			
		}

		if (event.type == sf::Event::MouseButtonReleased && (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right)) {
			is_moved = false;
		}

	}

	void update() {
		if (is_moved)
			setPosition(worldMousePosition - cam->position - offset);
		else
			setPosition(position);

	}

	void draw() {

		window->draw(bar_rect);
		window->draw(rect);
		
	}
	
};

Palette* palette = nullptr;

#endif // !palette_hpp
