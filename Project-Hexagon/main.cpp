#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#define M_PI 3.14159265358979323846 /* pi */

sf::RenderWindow *window;

float tile_radius = 24;

std::vector<sf::Vector3i> hex_neighbours = {
	 sf::Vector3i(+1, -1, 0),
	 sf::Vector3i(+1, 0, -1),
	 sf::Vector3i(0, +1, -1), 
	 sf::Vector3i(-1, +1, 0),
	 sf::Vector3i(-1, 0, +1),
	 sf::Vector3i(0, -1, +1)
};

class Tile {
  public:
	sf::Vector3i coords;
	sf::Color color = sf::Color(48,128,48);
	std::vector< Tile* > neighbours;
	
	Tile(int q, int r) {
		coords.x = q;
		coords.y = r;
		coords.z = -q - r;
		for(int dir=0;dir<6;dir++)
			neighbours.push_back(nullptr);
	}

	void draw() {
		sf::ConvexShape hexagon;
		hexagon.setPointCount(6);
		float x = tile_radius * std::sqrt(3.0f) * (float(coords.x) + 0.5f * float(coords.y));
		float y = tile_radius * 3.0f / 2.0f * float(coords.y);

		for (int i = 0; i < 6; i++) {
			float deg = i * 60 - 30;
			float rad = deg * M_PI / 180.0f;
			float xx = x + tile_radius * std::cos(rad);
			float yy = y + tile_radius * std::sin(rad);

			hexagon.setPoint(i, sf::Vector2f(xx, yy));
		}
		
		hexagon.setFillColor(color);
		hexagon.setOutlineThickness(3.0f);
		hexagon.setOutlineColor(sf::Color(48,48,48));
		window->draw(hexagon);
	}
};

class Map {
  public:
	std::vector<Tile *> tiles;
	
	Map() {
		create(6);
		//create(0,0,2,2);
	}
	
	Tile* getTile(int q, int r){
		for(auto& tile : tiles){
			if(tile->coords.x == q && tile->coords.y == r)
				return tile;
		}
		
		return nullptr;
	}
	
	Tile* getTile(int q, int r, int s){
		for(auto& tile : tiles){
			if(tile->coords.x == q && tile->coords.y == r && tile->coords.z == s)
				return tile;
		}
		
		return nullptr;
	}
	
	sf::Vector2f getSize() {
		if (tiles.empty())
			return sf::Vector2f(0.f, 0.f);

		float min_x = std::numeric_limits<float>::max();
		float max_x = std::numeric_limits<float>::lowest();
		float min_y = std::numeric_limits<float>::max();
		float max_y = std::numeric_limits<float>::lowest();

		for (auto& tile : tiles) {
			float x = tile_radius * std::sqrt(3.0f) * (float(tile->coords.x) + 0.5f * float(tile->coords.y));
			float y = tile_radius * 3.0f / 2.0f * float(tile->coords.y);

			for (int i = 0; i < 6; ++i) {
				float deg = i * 60 - 30;
				float rad = deg * M_PI / 180.0f;
				float xx = x + tile_radius * std::cos(rad);
				float yy = y + tile_radius * std::sin(rad);

				if (xx < min_x) min_x = xx;
				if (xx > max_x) max_x = xx;
				if (yy < min_y) min_y = yy;
				if (yy > max_y) max_y = yy;
			}
		}

		return sf::Vector2f(max_x - min_x, max_y - min_y);
	}

	void create(int left, int top, int right, int bottom) {
		for (int r = top; r < bottom; r++) { 
		    int r_offset = floor(r/2.0f); 
		    for (int q = left - r_offset; q < right - r_offset; q++) {
		        tiles.push_back(new Tile(q, r));
		    }
}
	}
	
	void create(int n) {
		for (int q = -n; q <= n; q++) {
			int r1 = std::max(-n, -q - n);
			int r2 = std::min(n, -q + n);

			for (int r = r1; r <= r2; r++) {
				tiles.push_back(new Tile(q, r));
			}
		}
		
		// center tile neighbours
		
		Tile* tile = getTile(0,0);
		//std::cout<<tile->coords.x<<","<<tile->coords.y<<","<<tile->coords.z<<"\n";
		for(int dir=0;dir<6;dir++){
			int q = tile->coords.x + hex_neighbours[dir].x;
			int r = tile->coords.y + hex_neighbours[dir].y;
			int s = tile->coords.z + hex_neighbours[dir].z;
			Tile* ngbr = tile->neighbours[dir];
			ngbr = getTile(q,r,s);
			if(ngbr!=nullptr)
				ngbr->color=sf::Color(128,48,48);
		}
		
	}

	void draw() {
		for (auto &tile : tiles) {
			tile->draw();
		}
	}
};

int main() {
	window = new sf::RenderWindow(sf::VideoMode(800,600), "Project Hexagon");
	window->setFramerateLimit(25);

	Map *mapa = new Map();

	while (window->isOpen()) {
		// Handle screen resizes
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();

		}
		
		sf::View v;
		v = window->getView();
		v.setCenter(0,0);
		window->setView(v);
		window->clear();
		mapa->draw();
		window->display();
	}
	return 0;
}
