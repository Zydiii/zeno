#ifndef __ZENO_EDITOR_DOCKWIDGET_H__
#define __ZENO_EDITOR_DOCKWIDGET_H__

#include "zenodockwidget.h"

class ZenoDockTitleWidget : public QWidget
{
	Q_OBJECT
public:
	ZenoDockTitleWidget(QWidget* parent = nullptr);
	~ZenoDockTitleWidget();
	QSize sizeHint() const override;
	void updateByType(DOCK_TYPE type);
	void setupUi();

signals:
	void dockOptionsClicked();
	void dockSwitchClicked(DOCK_TYPE);

protected:
	void paintEvent(QPaintEvent* event) override;
	virtual void initTitleContent(QHBoxLayout* pHLayout);

private slots:
	void onDockSwitchClicked();
};

class ZenoEditorDockTitleWidget : public ZenoDockTitleWidget
{
	Q_OBJECT
public:
	ZenoEditorDockTitleWidget(QWidget* parent = nullptr);
	~ZenoEditorDockTitleWidget();

protected:
	void initTitleContent(QHBoxLayout* pHLayout) override;

private:
	QMenuBar* initMenu();
};


#endif