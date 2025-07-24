#ifndef map_hpp
#define map_hpp

#define M_PI 3.14159265358979323846 /* pi */

float tile_radius = 24;

std::vector<sf::Vector3i> hex_neighbours = {
	sf::Vector3i(0, -1, +1),
	 sf::Vector3i(+1, -1, 0),
	 sf::Vector3i(+1, 0, -1),
	 sf::Vector3i(0, +1, -1),
	 sf::Vector3i(-1, +1, 0),
	 sf::Vector3i(-1, 0, +1)

};

bool isPointInTriangle(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f P) {
	float denominator = ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
	float w1 = ((B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y)) / denominator;
	float w2 = ((C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y)) / denominator;
	float w3 = 1.0f - w1 - w2;

	return (w1 >= 0) && (w2 >= 0) && (w3 >= 0);
}

class Tile {
public:
	sf::Vector3i coords;
	sf::Vector2f position;
	sf::Color color = sf::Color(48, 128, 48);
	sf::ConvexShape hexagon;
	std::vector< Tile* > neighbours;

	Tile(int q, int r) {
		coords.x = q;
		coords.y = r;
		coords.z = -q - r;

		position.x = tile_radius * 3.0f / 2.0f * float(coords.x);
		position.y = tile_radius * std::sqrt(3.0f) * (float(coords.y) + 0.5f * float(coords.x));

		for (int dir = 0; dir < 6; dir++)
			neighbours.push_back(nullptr);

		hexagon = sf::ConvexShape();
		hexagon.setPointCount(6);

		for (int i = 0; i < 6; i++) {
			float deg = i * 60 + 30;
			float rad = deg * M_PI / 180.0f;
			float xx = position.x + tile_radius * std::sin(rad);
			float yy = position.y - tile_radius * std::cos(rad);

			hexagon.setPoint(i, sf::Vector2f(xx, yy));
		}

		hexagon.setFillColor(color);
		hexagon.setOutlineThickness(3.0f);
		hexagon.setOutlineColor(sf::Color(48, 48, 48));
	}

	bool cursorHover() {

		for (int i = 0; i < 6; i++) {

			sf::Vector2f vc = position;
			sf::Vector2f v1 = hexagon.getPoint(i);
			sf::Vector2f v2 = hexagon.getPoint((i + 1) % 6);

			if (isPointInTriangle(vc, v1, v2, worldMousePosition)) {
				hexagon.setFillColor(sf::Color(128, 48, 8));
				return true;
			}
		}

		hexagon.setFillColor(color);
		return false;
	}

	void draw() {

		window->draw(hexagon);
	}
};

class Map {
public:
	std::vector<Tile*> tiles;
	Tile* selected_tile;

	Map() {
		create(6);
		//create(0,0,2,2);
	}

	Tile* getTile(int q, int r) {
		for (auto& tile : tiles) {
			if (tile->coords.x == q && tile->coords.y == r)
				return tile;
		}

		return nullptr;
	}

	Tile* getTile(int q, int r, int s) {
		for (auto& tile : tiles) {
			if (tile->coords.x == q && tile->coords.y == r && tile->coords.z == s)
				return tile;
		}

		return nullptr;
	}

	std::unordered_set<Tile*> getTiles(int q, int r, int s, int range) {
		std::unordered_set<Tile*> set;

		for (int qq = -range; qq <= range; qq++) {
			for (int rr = -range; rr <= range; rr++) {
				for (int ss = -range; ss <= range; ss++) {
					Tile* tile = getTile(q + qq, r + rr, s + ss);
					if (tile != nullptr)
						set.insert(tile);
				}
			}
		}

		return set;
	}

	std::unordered_set<Tile*> getTilesRing(int q, int r, int s, int ring_size) {
		std::unordered_set<Tile*> set;

		if (ring_size == 0) {
			Tile* center = getTile(q, r, s);
			if (center != nullptr)
				set.insert(center);
			return set;
		}

		int qq = q + hex_neighbours[4].x * ring_size;
		int rr = r + hex_neighbours[4].y * ring_size;
		int ss = s + hex_neighbours[4].z * ring_size;

		Tile* tt = getTile(qq, rr, ss);

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < ring_size; j++) {
				if (tt != nullptr)
					set.insert(tt);

				if (tt != nullptr)
					tt = tt->neighbours[i];
				else
					break; // przerwij jeśli nie ma sąsiada
			}
		}

		return set;
	}

	sf::Vector2f getSize() {
		if (tiles.empty())
			return sf::Vector2f(0.f, 0.f);

		float min_x = tiles[0]->hexagon.getPoint(0).x;
		float max_x = min_x;
		float min_y = tiles[0]->hexagon.getPoint(0).y;
		float max_y = min_y;

		for (auto& tile : tiles) {

			for (int i = 0; i < 6; ++i) {

				float xx = tile->hexagon.getPoint(i).x;
				float yy = tile->hexagon.getPoint(i).y;

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
			int r_offset = floor(r / 2.0f);
			for (int q = left - r_offset; q < right - r_offset; q++) {
				tiles.push_back(new Tile(q, r));
			}
		}
	}

	void create(int n) {
		for (int q = -n; q <= n; q++) {
			int r1 = max(-n, -q - n);
			int r2 = min(n, -q + n);

			for (int r = r1; r <= r2; r++) {
				tiles.push_back(new Tile(q, r));
			}
		}

		// center tile neighbours
		for (auto& tile : tiles) {
			for (int dir = 0; dir < 6; dir++) {
				int q = tile->coords.x + hex_neighbours[dir].x;
				int r = tile->coords.y + hex_neighbours[dir].y;
				int s = tile->coords.z + hex_neighbours[dir].z;
				tile->neighbours[dir] = getTile(q, r, s);
			}
		}

		for (auto& t : getTiles(0, 0, 0, 2))
			t->color = sf::Color(48, 48, 128);

		for (auto& t : getTilesRing(0, 0, 0, 6))
			t->color = sf::Color(64, 64, 64);

	}

	void cursorHover() {

		selected_tile = nullptr;

		for (auto& tile : tiles) {
			if (tile->cursorHover())
				selected_tile = tile;
		}
	}

	void draw() {
		for (auto& tile : tiles) {
			tile->draw();
		}
	}
};
#endif