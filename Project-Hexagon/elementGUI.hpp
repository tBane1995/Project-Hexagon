#ifndef elementGUI_hpp
#define elementGUI_hpp

class ElementGUI {
public:

	ElementGUI() { }
	~ElementGUI() { }

	void cursorHover();
	void handleEvent(sf::Event& event);
	void update();
	void draw();
};

ElementGUI* ElementGUI_hovered = nullptr;
ElementGUI* ElementGUI_pressed = nullptr;

#endif