#include "StdAfx.h"
#include "GameState_DrawingState.h"


GameState_DrawingState::GameState_DrawingState(const sf::Window& window)
	:GameStateBase(window),
	m_colorPicker(nullptr),
	m_rectangle(nullptr),
	m_drawingBoard(nullptr),
	m_drawingWindow(nullptr),
	m_colorSelector(nullptr)
{
}


GameState_DrawingState::~GameState_DrawingState(void)
{
}

void GameState_DrawingState::OnAwake(const SFMLStateInfo* lStateInfo)
{
	std::unique_ptr<SFMLColorPalette> colorPicker(new SFMLColorPalette(m_window, ImageManager::getInstance().getImage("ColorWheel"), 
		TextureManager::getInstance().getTexture("ColorWheel")));
	m_colorPicker = colorPicker.get();
	colorPicker->addMouseLeftClickedFunction(std::make_shared<TFunctor<GameState_DrawingState>>(this, &GameState_DrawingState::updateColorSelector));
	colorPicker->setPosition(0.0f, 300.0f);
	std::unique_ptr<sf::RectangleShape> colorSelected(new sf::RectangleShape(sf::Vector2f(20.0f, 20.0f)));
	m_rectangle = colorSelected.get();
	colorSelected->setPosition(150.0f, 150.0f);
	colorSelected->setOutlineThickness(2.0f);
	colorSelected->setOutlineColor(sf::Color::White);
	
	std::unique_ptr<SFMLCursesWindow> cursesWindow(new SFMLCursesWindow(m_window, sf::Vector2i(4,10)));
	m_drawingBoard = cursesWindow.get();
	cursesWindow->clearTiles("s", sf::Color::Blue, sf::Color::Green);
	cursesWindow->setTile(SFMLCursesChar(m_window, "a", sf::Color::Red, sf::Color::Cyan), sf::Vector2i(2,6));
	cursesWindow->setPosition(400,200);

	std::unique_ptr<DrawingWindow> drawingWindow(new DrawingWindow(m_window, sf::Vector2i(10,10)));
	m_drawingWindow = drawingWindow.get();
	drawingWindow->setPosition(20.0f,20.0f);

	std::unique_ptr<ColorSelector> colorSelector(new ColorSelector(m_window));
	m_colorSelector = colorSelector.get();
	colorSelector->setPosition(400.0f, 400.0f);

	addGUIElement(std::move(colorPicker));
	addDrawable(std::move(colorSelected));
	addGUIElement(std::move(cursesWindow));
	addGUIElement(std::move(drawingWindow));
	addGUIElement(std::move(colorSelector));
}
void GameState_DrawingState::OnUpdate(void)
{
}
void GameState_DrawingState::OnRender(sf::RenderTarget& target)
{
	GameStateBase::drawDisplayList(target);
}
void GameState_DrawingState::OnCleanup(void)
{
	m_colorPicker = nullptr;
	m_rectangle = nullptr;
	m_drawingBoard = nullptr;
	m_drawingWindow = nullptr;
	m_colorSelector = nullptr;
	GameStateBase::Cleanup();
}
void GameState_DrawingState::OnSuspend(void)
{
}
void GameState_DrawingState::OnResume(void)
{
}

void GameState_DrawingState::saveAPF(const std::string& fileName)
{
	std::string extFileName(fileName);
	extFileName += ".apf";
	std::ofstream write(extFileName, std::ios::out);
	write<<m_drawingWindow->getCursesWindow();
	write.close();
}

void GameState_DrawingState::loadAPF(const std::string& fileName)
{
	std::string extFileName(fileName);
	extFileName += ".apf";
	std::ifstream read(extFileName, std::ios::in);
	if(read.is_open())
	{
		read>>m_drawingWindow->getCursesWindow();
		read.close();
	}
	else
	{
		//throw an exception here instead
		printf("FAILED TO OPEN FILE %s\n", extFileName.c_str());
	}

}

void GameState_DrawingState::updateColorSelector()
{
	m_colorSelector->setPrimaryColor(m_colorPicker->getSelectedColor());
}

void GameState_DrawingState::OnKeyPressed(sf::Keyboard::Key key, bool alt, bool control, bool shift)
{
	switch(key)
	{
	case sf::Keyboard::Up:
		m_drawingWindow->moveCursorUp();
		break;
	case sf::Keyboard::Down:
		m_drawingWindow->moveCursorDown();
		break;
	case sf::Keyboard::Left:
		m_drawingWindow->moveCursorLeft();
		break;
	case sf::Keyboard::Right:
		m_drawingWindow->moveCursorRight();
		break;
	case sf::Keyboard::S:
		saveAPF("test");
		break;
	case sf::Keyboard::L:
		loadAPF("test");
		break;
	case sf::Keyboard::Escape:
		m_messages.push_back(new SFMLStateMessage_Close());
		break;
	}
}

void GameState_DrawingState::OnTextEntered(sf::Uint32 text)
{
	m_drawingWindow->setCursorCharacter(SFMLCursesChar(m_window,std::string(sf::String(text)),
		m_colorSelector->getPrimaryColor(), m_colorSelector->getSecondaryColor()));
}