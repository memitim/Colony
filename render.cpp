#include "render.h"



// Default map panning speed
int Render::panSpeed = 16;

// Default minimap scale compared to main map
static float miniMapScale = 0.27f;

// Initialize viewports
sf::View Render::mainMapView;
sf::View Render::miniMapView;
sf::View Render::controlsView;

// Other static variable definitions

int Render::currentDepth = 5;
sf::Vector2i Render::currCorner(0, 0);
bool Render::isControlSelected = false;
int Render::selectedControl;

sf::RectangleShape Render::plainTile;
sf::RectangleShape Render::outlineTile;
sf::RectangleShape Render::hoverOutlineTile;
sf::Text Render::infoText;
std::vector<std::vector <sf::Sprite> > Render::spriteTiles;
std::vector<std::vector <sf::Texture> > Render::texture;

Render::Render()
{
	
}

Render::~Render()
{

}

void Render::prepGraphics(Window & window)
	/**
	* Performs the initial configuration of the render variables.
	*/
{
	this->loadTextures();
    // Outline breaking arrayMap. Need to determine cause (see setSelectedControl method for other function that needs to be enabled after fix)
	this->createTileOutline();
    
	// Set up main map view and viewport
	this->mainMapView.reset(sf::FloatRect((float)textureDim,
	(float)textureDim,
	(float)mapPaneWidth * (float)textureDim - (float)textureDim, 
	(float)mapPaneHeight * (float)textureDim - (float)textureDim));
	this->mainMapView.setViewport(sf::FloatRect((float)leftBuffer / (float)window.getSize().x, 
		(float)topBuffer / (float)window.getSize().y, 
		((float)mapPaneWidth * (float)textureDim) / (float)window.getSize().x, 
		((float)mapPaneHeight * (float)textureDim) / (float)window.getSize().y));

	// Set up minimap view and viewport
	this->miniMapView = mainMapView;
	this->miniMapView.setViewport(sf::FloatRect((((float)mapPaneWidth * (float)textureDim * 1.f) + (float)paneBufferX + (float)leftBuffer) / (float)window.getSize().x, 
		(float)topBuffer / (float)window.getSize().y, 
		(float)mapPaneWidth * (float)textureDim * miniMapScale / (float)window.getSize().x * 1.f, 
		(float)mapPaneHeight * (float)textureDim * miniMapScale / (float)window.getSize().y * 1.f));

	// Set up controls view and viewport
	controlsView.reset(sf::FloatRect(0.f,0.f, (float)numTilesControls * (float)textureDim, 
		(floor((float)numTiles / (float)numTilesControls + 1.f) * (float)textureDim) * 1.00f));
	controlsView.setViewport(sf::FloatRect((((float)mapPaneWidth * (float)textureDim * 1.f) + (float)paneBufferX + (float)leftBuffer) / (float)window.getSize().x, 
		((float)mapPaneHeight * (float)textureDim * miniMapScale * 1.f + (float)topBuffer + (float)paneBufferY) / (float)window.getSize().y, 
		((float)numTilesControls * (float)textureDim) / (float)window.getSize().x * 1.f, 
		(floor((float)numTiles / (float)numTilesControls + 1.f) * (float)textureDim) * 1.00f / (float)window.getSize().y));
    
}

// Create the outlines used for tiles
void Render::createTileOutline()
{
	this->plainTile.setSize(sf::Vector2f(float(this->textureDim),float(this->textureDim)));
	this->outlineTile.setSize(sf::Vector2f(float(this->textureDim),float(this->textureDim)));
	this->outlineTile.setOutlineColor(sf::Color::Black);
	this->outlineTile.setOutlineThickness(1);
	this->outlineTile.setFillColor(sf::Color::Transparent);
	this->hoverOutlineTile = this->outlineTile;
	this->hoverOutlineTile.setOutlineColor(sf::Color(48,48,48,192));
}

// Load textures from files and create sprites
void Render::loadTextures()
{
	this->spriteTiles.resize(this->numTiles);
	this->texture.resize(this->numTiles);
	for(int ns=0;ns<this->numTiles;++ns)
	{
		this->spriteTiles[ns].resize(this->numTileAnimations);
		this->texture[ns].resize(this->numTileAnimations);
	}
	
	// Load the sprite image from a file
	std::string textureSource;
	for(int a=0;a<this->numTiles;a++)
	{
		for(int b=0;b<this->numTileAnimations;++b)
		{
			textureSource = "texture" + (static_cast<std::ostringstream*>( &(std::ostringstream() << a + 1) )->str()) + "-" + (static_cast<std::ostringstream*>( &(std::ostringstream() << b) )->str()) +".png";
			if (!texture[a][b].loadFromFile(textureSource))
			{
				// Need error handler
				std::cout << "OH CRAP! Texture load issue!" << std::endl;
			}
			this->spriteTiles[a][b].setTexture(texture[a][b]);
		}
	}
}



// Write the screen elements to their respective views
void Render::drawMap(Window & window, Worldmap & worldmap)
{
	static int frameCounter = 0;
	static sf::Clock clock;
	// Limit animations to <some> frames per second regardless of framerate
	// TODO: Nail down a consistent equation of coverting FPS to that constant on the end
	bool animate = false;
	if(frameCounter < 1.f / clock.getElapsedTime().asMilliseconds() * 1000 / 0.2f)
	{
		frameCounter++;
	}
	else
	{
		animate = true;
		clock.restart();
		frameCounter = 0;
	}

	// Draw the main map and minimap
	for (int x = 0 + (int)(this->currCorner.x / this->textureDim); x < this->mapPaneWidth + 2 + (int)(this->currCorner.x / this->textureDim); x++)
	{
		for (int y = 0 + (int)(this->currCorner.y / this->textureDim); y < this->mapPaneHeight + 2 + (int)(this->currCorner.y / this->textureDim); y++)
		{
			if(worldmap.mapArray[x][y][this->currentDepth][0] > -1)
			{
				int currentAnimationTile = worldmap.mapArray[x][y][this->currentDepth][1];
                this->spriteTiles[worldmap.mapArray[x][y][this->currentDepth][0]][currentAnimationTile].setPosition(float(this->textureDim * x),
					float(textureDim * y));
				window.setView(this->mainMapView);
                window.draw(this->spriteTiles[worldmap.mapArray[x][y][this->currentDepth][0]][currentAnimationTile]);

                spriteTiles[worldmap.mapArray[x][y][this->currentDepth][0]][0].setPosition(float(this->textureDim * x),
					float(this->textureDim * y));
				window.setView(this->miniMapView);
                window.draw(this->spriteTiles[worldmap.mapArray[x][y][this->currentDepth][0]][0]);

				// Toggle the tile animation if sufficient frames have passed
				if (animate == true)
				{
					// Rotate to the next animation frame for the tile
					if (currentAnimationTile == this->numTileAnimations - 1)
					{
                        worldmap.mapArray[x][y][this->currentDepth][1] = 0;
					}
					else
					{
                        worldmap.mapArray[x][y][this->currentDepth][1] = currentAnimationTile + 1;
					}
				}
			}
			else
			{
				int renderDepth = currentDepth;
				int renderAlpha = 192;
				bool layerFound = false;
				do
				{
					renderDepth++;
                    if (worldmap.mapArray[x][y][renderDepth][0] > -1)
					{
                        this->spriteTiles[worldmap.mapArray[x][y][renderDepth][0]][0].setColor(sf::Color(255, 255, 255, renderAlpha));
                        this->spriteTiles[worldmap.mapArray[x][y][renderDepth][0]][0].setPosition(float(this->textureDim * x),
							float(textureDim * y));
						window.setView(this->mainMapView);
                        window.draw(this->spriteTiles[worldmap.mapArray[x][y][renderDepth][0]][0]);

						window.setView(this->miniMapView);
                        window.draw(this->spriteTiles[worldmap.mapArray[x][y][renderDepth][0]][0]);
                        this->spriteTiles[worldmap.mapArray[x][y][renderDepth][0]][0].setColor(sf::Color(255, 255, 255, 255));
						layerFound = true;
                        renderDepth = worldmap.mapDepth;
					}
					renderAlpha -= 64;
                } while (renderDepth < worldmap.mapDepth && renderAlpha > 0);

				if(layerFound = false)
				{
                    this->spriteTiles[worldmap.mapArray[x][y][currentDepth][0]][0].setColor(sf::Color(0, 0, 0, 255));
                    this->spriteTiles[worldmap.mapArray[x][y][currentDepth][0]][0].setPosition(float(this->textureDim * x),
						float(textureDim * y));
					window.setView(this->mainMapView);
                    window.draw(this->spriteTiles[worldmap.mapArray[x][y][currentDepth][0]][0]);

					window.setView(this->miniMapView);
                    window.draw(this->spriteTiles[worldmap.mapArray[x][y][currentDepth][0]][0]);
				}
			}
		}
	}
	
	// Draw the hover tile
	window.setView(this->mainMapView);
	window.draw(this->hoverOutlineTile);
	
	
	// Draw the controls
	window.setView(this->controlsView);
	for (int controls = 0; controls < this->numTiles; controls++)
	{
		int row = controls / this->numTilesControls;
		this->spriteTiles[controls][0].setPosition(float(textureDim * controls - (this->numTilesControls * row * this->textureDim)), float(row * this->textureDim + row));
		this->outlineTile.setPosition(this->spriteTiles[controls][0].getPosition());
		window.draw(this->spriteTiles[controls][0]);
		window.draw(this->outlineTile);
	}

	window.setView(window.getDefaultView());

	// Render informational text at top of screen
	window.draw(this->infoText);
}

// Process left-clicks that reach the main screen
void Render::leftClickScreen(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap)
{
	// Test for clicks in the controls viewport
	if(mousePosition.x > controlsView.getViewport().left * window.getSize().x
		&& mousePosition.x < (controlsView.getViewport().left + controlsView.getViewport().width) * window.getSize().x
		&& mousePosition.y > controlsView.getViewport().top * window.getSize().y
		&& mousePosition.y < (controlsView.getViewport().top + controlsView.getViewport().height) * window.getSize().y)
	{
		Render::setSelectedControl(window, mousePosition);
	}
	
	// Test for clicks in the main map and if a control is selected, then place control on selected tile
	if(isControlSelected == true && mousePosition.x > leftBuffer && mousePosition.x < leftBuffer + (mapPaneWidth * textureDim)
		&& mousePosition.y > topBuffer && mousePosition.y < topBuffer + (mapPaneHeight * textureDim))
	{
		Render::setSelectedTile(window, mousePosition, worldmap);
	}
}

// Set the location of the hover outline
void Render::checkHover(Window & window, sf::Vector2i mousePosition)
{
	if(mousePosition.x > leftBuffer && mousePosition.x < leftBuffer + (mapPaneWidth * textureDim)
		&& mousePosition.y > topBuffer && mousePosition.y < topBuffer + (mapPaneHeight * textureDim))
	{
		window.setView(mainMapView);
		sf::Vector2f convertHoverTile = window.mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));	
		hoverOutlineTile.setPosition(textureDim * floor(convertHoverTile.x / (float)textureDim), textureDim * floor(convertHoverTile.y / (float)textureDim));
		window.setView(window.getDefaultView());
	}
	else
		hoverOutlineTile.setPosition (0.f,0.f);
}

// Set the chosen control if control viewport clicked
void Render::setSelectedControl(Window & window, sf::Vector2i mousePosition)
{
	isControlSelected = true;
	window.setView(controlsView);
	int row = numTiles / numTilesControls;
    sf::Vector2f convertSelectedControl = window.mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	int select = (int)(convertSelectedControl.x / textureDim) + (numTilesControls * (int)(convertSelectedControl.y / textureDim));

    if(select < numTiles)
	{
		selectedControl = select;
		hoverOutlineTile = plainTile;
		hoverOutlineTile.setTexture(&texture[selectedControl][0]);
		hoverOutlineTile.setFillColor(sf::Color(255,255,255,160));
        
	}
	window.setView(window.getDefaultView());
}

// Process screen scrolling keypresses


// Set the sprite of the main map tile clicked to the control currently selected
void Render::setSelectedTile(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap)
	/**
	* Sets the tile on the cursor that has been chosen from the controls.
	*/
{
	window.setView(mainMapView);
    sf::Vector2f convertSelectedTile = window.mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	this->setTile(sf::Vector2i(floor(convertSelectedTile.x / textureDim), floor(convertSelectedTile.y / textureDim)),worldmap);
	window.setView(window.getDefaultView());
}

void Render::drawText(Window & window, sf::Vector2i mousePosition)
{
	sf::Font font;
	font.loadFromFile("arial.ttf");

	std::string xPos = static_cast<std::ostringstream*>( &(std::ostringstream() << mousePosition.x))->str();
	std::string yPos = static_cast<std::ostringstream*>( &(std::ostringstream() << mousePosition.y))->str();
	infoText.setString("X: " + xPos + ", Y: " + yPos);
	infoText.setStyle(sf::Text::Bold);
	infoText.setCharacterSize(18);
	infoText.setColor(sf::Color::White);
	infoText.setPosition(0.f, 0.f);
}
// Render all graphics
void Render::drawScreen(Window & window, Worldmap & worldmap)
{
	window.clear();	
	drawMap(window, worldmap);
	window.display();
}

void Render::releaseSelectedControl(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap)
{
    if (this->isControlSelected == false)
    {
        // Test for clicks in the main map and if a control is selected, then place control on selected tile
        if (mousePosition.x > leftBuffer && mousePosition.x < leftBuffer + (mapPaneWidth * textureDim)
            && mousePosition.y > topBuffer && mousePosition.y < topBuffer + (mapPaneHeight * textureDim))
        {
            Render::digHole(window, mousePosition, worldmap);
        }
    }

    if(this->isControlSelected == true) 
	{
		this->isControlSelected = false;
        
		this->hoverOutlineTile = this->outlineTile;
		this->hoverOutlineTile.setOutlineColor(sf::Color(48,48,48,192));
        
	}

}

void Render::digHole(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap)
{
	window.setView(mainMapView);
    sf::Vector2f convertSelectedTile = window.mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	sf::Vector2i digPosition = sf::Vector2i(floor(convertSelectedTile.x / textureDim), floor(convertSelectedTile.y / textureDim));
    worldmap.mapArray[digPosition.x][digPosition.y][currentDepth][0] = -1;
    worldmap.mapArray[digPosition.x][digPosition.y][currentDepth][1] = 0;
	window.setView(window.getDefaultView());
}

void Render::changeDepth(sf::Event event, Worldmap & worldmap) {
    if ((event.key.code == sf::Keyboard::LBracket) && (this->currentDepth > 0))
        this->currentDepth--; else if ((event.key.code == sf::Keyboard::RBracket) && (this->currentDepth < worldmap.mapDepth - 1)) this->currentDepth++;
}
void Render::panLeft() {
    if (this->currCorner.x > 0)	{
        this->currCorner.x = this->currCorner.x - this->panSpeed; this->mainMapView.move(float(-panSpeed), 0.f);
        this->miniMapView.move(float(-this->panSpeed), 0.f);
    }
}
void Render::panRight(Worldmap & worldmap) {
    if (this->currCorner.x < ((worldmap.mapWidth * this->textureDim) - (this->mapPaneWidth * this->textureDim)) - this->panSpeed - this->textureDim)
    {
        this->currCorner.x = this->currCorner.x + this->panSpeed; this->mainMapView.move(float(panSpeed), 0.f); this->miniMapView.move(float(this->panSpeed), 0.f);
    }
}
void Render::panUp() {
    if (this->currCorner.y > 0) {
        this->currCorner.y = this->currCorner.y - this->panSpeed; this->mainMapView.move(0.f, float(-this->panSpeed));
        this->miniMapView.move(0.f, float(-this->panSpeed));
    }
}
void Render::panDown(Worldmap & worldmap) {
    if (this->currCorner.y < ((worldmap.mapHeight * this->textureDim) - (this->mapPaneHeight * this->textureDim)) - this->panSpeed - this->textureDim)
    {
        this->currCorner.y = this->currCorner.y + this->panSpeed; this->mainMapView.move(0.f, float(this->panSpeed)); this->miniMapView.move(0.f, float(this->panSpeed));
    }
}

void Render::setTile(sf::Vector2i newTile, Worldmap & worldmap) {worldmap.mapArray[newTile.x][newTile.y][this->currentDepth][0] = this->selectedControl; }