#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QMenu>
#include <QAction>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include <scene/camera.h>
#include <scene/scene.h>
#include <ProgPreviewWidget.h>
#include <raytracer/RaytraceEngine.h>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

public slots:

private:
	RaytraceEngine re;
	Scene *scene;
	Camera *camera;

	// GUI variables
	void setAllOptionsEnabled(bool enabled);
	ProgPreviewWidget *imgDisplay;
	QLabel *imgLabel;
	QPixmap pixmap;
	QImage img;

	void createActions();
	void createMenus();
	QMenu *fileMenu;
	QAction *loadSceneAct;
	QAction *renderAct;
	QAction *saveImageAct;

	void createOptionDock();
	QDockWidget *optionDock;
	QGroupBox *optionDockContents;
	QSpinBox *recursionDepthBox;
	QCheckBox *multithreadingBox;
	QCheckBox *kdTreeBox;
	QSpinBox *AABox;

	QGroupBox *AOSettings;
	QSpinBox *AOSamplesBox;
	QDoubleSpinBox *AOSpreadBox;
	QDoubleSpinBox *AODistanceBox;
	QPushButton *AOButton;

	QLabel *recursionDepthLabel;
	QLabel *multithreadingLabel;
	QLabel *kdTreeLabel;
	QLabel *AALabel;
	QLabel *AOSamplesLabel;
	QLabel *AOSpreadLabel;
	QLabel *AODistanceLabel;

	void createStatsDock();
	QDockWidget *statsDock;
	QGroupBox *statsDockContents;
	QLabel *renderTimeLabel;
	QLabel *renderTimeValue;
	QLabel *kdTreeTimeLabel;
	QLabel *kdTreeTimeValue;
	QLabel *trisLabel;
	QLabel *trisValue;
	QLabel *geoIsectLabel;
	QLabel *geoIsectValue;
	QLabel *bboxIsectLabel;
	QLabel *bboxIsectValue;

	QString formatMs(qint64 ms);
private slots:
	void loadScene();
	void renderScene();
	void saveImage();
	void genAOPass();
};

#endif // __MAINWINDOW_H__