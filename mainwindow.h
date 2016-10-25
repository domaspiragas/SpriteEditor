#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "colorpicker.h"
#include "toolDefinitions.h"

namespace Ui {
class MainWindow;
}

class Canvas;
class PreviewWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void SetTool(TOOL_ACTIONS);
    void StartExportGIF(const QString&);
    void EndExportGIF();
    void SetActiveFrame(int); // Param: index of frame
    void AddFrame();
    void DeleteFrameSignal(int); // Param: index of frame to be deleted
    void AddLayer(int); // Param: index of frame
    void DeleteLayerSignal(int, int); // Param: frame index, layer index
    void RepaintRequest();
    void SelectFrameSignal(int);
    void CurrentFrame(int); // to send which frame to duplicate
    void ReverseAnimationSignal();
    void RepaintPreviewWindow(); // Emit this when the frames change to repaint the preview window
    void PreviewWindowIsDisplayed(bool); // Emit true when the preview window is displayed, false otherwise

private slots:
    void on_colorButton_clicked();
    void on_colorButtonFill_clicked();
    void CreateNew(); // Called when user clicks File > New to create a new canvas to draw on
    void RetrieveNewFormInfo(int width, int height, QColor color);
    void ExportGIF();
    void on_lineButton_clicked();
    void on_rectButton_clicked();
    void on_pencilButton_clicked();
    void ExportGIFStart(const QString&);
    void ExportGifEnd();
    void SaveProjectFile();
    void SaveProjectFileLastName();
    void OpenFile();

    // Called when the selected frame changes, tells canvas which frame to display and draw on
    void SelectedFrame();

    // Called when the selected layer/frame changes, tells canvas which layer to display and draw on
    void SelectedLayer();
    void on_add_layer_button_clicked();
    void on_delete_layer_button_clicked();
    void on_delete_frame_button_clicked();
    void on_add_frame_button_clicked();
    void on_duplicate_frame_button_clicked();
    void on_eraserButton_clicked();
    void on_ellipseButton_clicked();
    void on_add_reverse_animation_button_clicked();
    void on_brush_size_spinbox_valueChanged(int value);

    void showPreview(bool);
    void UpdateFPS(int);
    void closePreview(bool);

    void on_paintBucketButton_clicked();
    void on_sprayButton_clicked();

    void Resize();

private:
    void BindCanvas(int, int, QColor);
    void BindCanvas();

    std::string* m_lastFileSave;

    Ui::MainWindow *ui;
    Canvas* m_canvas;
    ColorPicker* m_color;
    void ColorChange(QColor color);
    std::string GetRGB(QColor color);
    QDialog* m_newFormDialog;

    int getNumFrames(); // Gets the number of frames from the canvas
    void updateFrameUI(); // Updates the frame list in the UI based on the total number of frames

    int getNumLayers(int frameIndex); // Gets the number of layers from the currently selected frame

    // Updates the layer list in the UI based on the number of layers for the selected frame
    void updateLayerUI(bool);

    PreviewWindow* m_previewWindow;

//    void Resize(int,int); // Helps the slot
};

#endif // MAINWINDOW_H
