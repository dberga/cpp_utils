#ifndef ExitWidget_hxx
#define ExitWidget_hxx

#include <QtGui>
#include <string>

class ExitWidget : public QWidget
{
private:
	QLabel _exit;
	QLabel _direction;
	QGridLayout _layout;
public:
	ExitWidget(const std::string & name)
		: _direction(QString::fromStdString(name + ":"))
		, _layout(this)
	{
		setFixedSize(100, 100);
		_layout.addWidget(&_direction);
		_layout.addWidget(&_exit);
		_layout.setAlignment(Qt::AlignCenter);
	}

	void update(const std::string & exit)
	{
		_exit.setText(QString::fromStdString(exit));
	}
	
};

#endif
