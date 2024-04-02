#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void newScreenshot();
    void saveScreenshot();
    void shootScreen();
    void updateCheckBox();

private:
    void updateScreenshotLabel();

    QPixmap originalPixmap;

    QLabel *screenshotLabel;
    QSpinBox *delaySpinBox;
    QCheckBox *hideThisWindowCheckBox;
    QPushButton *newScreenshotButton;
};

#endif // SCREENSHOT_H
