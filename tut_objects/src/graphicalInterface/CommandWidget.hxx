#ifndef CommandWidget_hxx
#define CommandWidget_hxx

#include <QtGui>
#include <string>
#include <vector>

class GameInterface;

class CommandWidget : public QWidget
{
	QGridLayout _layout;
	QPushButton _south;
	QPushButton _north;
	QPushButton _east;
	QPushButton _west;
	QPushButton _use;
	QLabel _itemLabel;
	QComboBox _items;
	QLabel _characterLabel;
	QComboBox _characters;

public:
	CommandWidget(GameInterface * game);

	std::string getItem()
	{
		return _items.currentText().toStdString();
	}
	
	void updateItems(std::vector<std::string> & items)
	{
		_items.clear();
		for (std::vector<std::string>::iterator it = items.begin(); it != items.end(); ++it)
			_items.addItem(QString::fromStdString(*it));
	}
	
	void updateCharacters(std::vector<std::string> & chars)
	{
		_characters.clear();
		for (std::vector<std::string>::iterator it = chars.begin(); it != chars.end(); ++it)
			_characters.addItem(QString::fromStdString(*it));
	}
};

#endif
