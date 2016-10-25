#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <iostream> // TODO: DELETE, used for debugging
#include "gifexporter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <newformdialog.h>
#include "projectfile.h"
#include <QShortcut>
#include "previewwindow.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_color = 0;
    m_lastFileSave = 0;
    m_newFormDialog = 0;
    ui->setupUi(this);

    //handle spacing of widgets
    ui->centralWidget->layout()->setSpacing(5);
    ui->centralWidget->layout()->setMargin(5);

    // initial color display
    ui->colorButton->setStyleSheet("background-color:black; border:1px solid #00ffff");
    ui->colorButtonFill->setStyleSheet("background-color:white; border:1px solid #00ffff");
    // we don't use status bar
    ui->statusBar->hide();
    ui->menuBar->setStyleSheet("QMenuBar{background-color:#404040; color:#00ffff;font-family:bauhaus 93;} QMenuBar::item{background:#404040;} QMenu::item:selected{border-color:#00ffff; background:#000000}");
    ui->menuFile->setStyleSheet("background-color:#404040; color:#00ffff; font-family:bauhaus 93;");
    ui->menuWindow->setStyleSheet("background-color:#404040; color:#00ffff; font-family:bauhaus 93;");

    //Set Tool Tips for all buttons
    ui->zoom_in->setToolTip("Zoom In");
    ui->zoom_out->setToolTip("Zoom Out");
    ui->pencilButton->setToolTip("Pencil");
    ui->eraserButton->setToolTip("Eraser");
    ui->lineButton->setToolTip("Line Tool");
    ui->ellipseButton->setToolTip("Ellipse Tool");
    ui->sprayButton->setToolTip("Spray Tool");
    ui->paintBucketButton->setToolTip("Paint Bucket Tool");
    ui->colorButton->setToolTip("Primary Color");
    ui->colorButtonFill->setToolTip("Fill Color");
    ui->add_frame_button->setToolTip("Add Frame");
    ui->delete_frame_button->setToolTip("Delete Selected Frame");
    ui->duplicate_frame_button->setToolTip("Duplicate Selected Frame");
    ui->add_reverse_animation_button->setToolTip("Add Reverse Animation");
    ui->add_layer_button->setToolTip("Add Layer");
    ui->delete_layer_button->setToolTip("Delete Selected Frame");
    ui->increase_fps_button->setToolTip("Increase Frames Per Second");
    ui->decrease_fps_button->setToolTip("Decrease Frames Per Second");

    m_newFormDialog = new newformdialog;

    m_canvas = 0;
    ui->scrollCanvas->setBackgroundRole(QPalette::Dark);
    ui->preview_window_dock->setHidden(true);
    /*The left and right docks extended to the bottom look better than
      the bottom dock being extended across the entire bottom */
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    BindCanvas(50,50,QColor(255,255,255,255));

    // Enable option to reopen docks when closed
    connect (ui->actionOpen_Tools, SIGNAL(triggered(bool)), ui->tools_dock, SLOT(setHidden(bool)));
    connect (ui->actionOpen_Layers_Window, SIGNAL(triggered(bool)), ui->layers_dock, SLOT(setHidden(bool)));
//    connect (ui->actionOpen_Preview_Window, SIGNAL(triggered(bool)), ui->preview_window_dock, SLOT(setHidden(bool)));
    connect (ui->actionExport_GIF, SIGNAL(triggered(bool)), this, SLOT(ExportGIF()));
    connect (ui->actionSave_As, SIGNAL(triggered(bool)), this, SLOT(SaveProjectFile()));
    connect (ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(SaveProjectFileLastName()));
    connect (ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(OpenFile()));

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(CreateNew()));

    connect(m_newFormDialog, SIGNAL(SendNewFormInfo(int, int, QColor)), this, SLOT(RetrieveNewFormInfo(int,int,QColor)));

    connect(this, SIGNAL(StartExportGIF(QString)), this, SLOT(ExportGIFStart(QString)));
    connect(this, SIGNAL(EndExportGIF()), this, SLOT(ExportGifEnd()));

    // When the selected frame changes, notify the canvas > frame so it's displayed/drawn on correctly
    connect(ui->frames_list, SIGNAL(itemSelectionChanged()),this,SLOT(SelectedFrame()));

    // When the selected frame/layer changes, notify the canvas > frame which layer is now active to draw on
    connect(ui->layers_list, SIGNAL(itemSelectionChanged()),this,SLOT(SelectedLayer()));
    connect(ui->frames_list, SIGNAL(itemSelectionChanged()),this,SLOT(SelectedLayer()));

    //Keyboard shortcuts
    QShortcut *brushShortcut = new QShortcut(QKeySequence("Ctrl+b"), this);
    connect(brushShortcut, SIGNAL(activated()),this, SLOT(on_pencilButton_clicked()) );

    QShortcut *rectShortcut = new QShortcut(QKeySequence("Ctrl+r"), this);
    connect(rectShortcut, SIGNAL(activated()),this, SLOT(on_rectButton_clicked()) );

    QShortcut *eraserShortcut = new QShortcut(QKeySequence("Ctrl+e"), this);
    connect(eraserShortcut, SIGNAL(activated()),this, SLOT(on_eraserButton_clicked()) );

    QShortcut *lineShortcut = new QShortcut(QKeySequence("Ctrl+l"), this);
    connect(lineShortcut, SIGNAL(activated()),this, SLOT(on_lineButton_clicked()) );

    QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+s"), this);
    connect(saveShortcut, SIGNAL(activated()),this, SLOT(SaveProjectFile()) );

    QShortcut *saveAsShortcut = new QShortcut(QKeySequence("Ctrl+Shift+s"), this);
    connect(saveAsShortcut, SIGNAL(activated()),this, SLOT(SaveProjectFileLastName()) );

    connect(ui->actionResize, SIGNAL(triggered()), this, SLOT(Resize()));

    m_newFormDialog->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_canvas;
    delete m_lastFileSave;
    delete m_color;
    delete m_newFormDialog;
    delete m_previewWindow;
}

void MainWindow::Resize()
{
    QDialog dialog(this);
    dialog.setStyleSheet("QDialog{background-color:#404040; boder: 2px solid #00ffff;}");
    dialog.setWindowTitle("Resize Dialog");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    QSpinBox *spin = new QSpinBox(&dialog);
    spin->setStyleSheet("QSpinBox{background-color:#262626;color:#00ffff;border:1px solid #00ffff;	font: 8pt \"Bauhaus 93\";}");
    spin->setMinimum(1);
    spin->setMaximum(250);
    QLabel *label = new QLabel(&dialog);
    label->setText("Width In Pixels (1 - 250");
    label->setStyleSheet("QLabel{color:#00ffff; font: 10pt \"Bauhaus 93 \"}");
    form.addRow(label,spin);

    QSpinBox *spin2 = new QSpinBox(&dialog);
    spin2->setStyleSheet("QSpinBox{background-color:#262626;color:#00ffff;border:1px solid #00ffff;	font: 8pt \"Bauhaus 93\";}");
    spin2->setMinimum(1);
    spin2->setMaximum(250);
    QLabel* label2 = new QLabel(&dialog);
    label2->setText("Height In Pixels (1 - 250");
    label2->setStyleSheet("QLabel{color:#00ffff; font: 10pt \"Bauhaus 93 \"}");
    form.addRow(label2, spin2);


    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted)
    {
        int width = spin->value();
        int height = spin2->value();

        // Calling this method does all the back-end work
        m_canvas->Resize(width, height);
    }
}

void MainWindow::closePreview(bool visible)
{
    if (!visible)
    {
        ui->preview_window_dock->setHidden(true);
        emit PreviewWindowIsDisplayed(false);
    }
}

void MainWindow::UpdateFPS(int fps)
{
    ui->fps_label->setText(QString::number(fps));
    if (fps > 29)
        ui->increase_fps_button->setDisabled(true);
    else
        ui->increase_fps_button->setDisabled(false);
    if (fps < 2)
        ui->decrease_fps_button->setDisabled(true);
    else
        ui->decrease_fps_button->setDisabled(false);
}

void MainWindow::showPreview(bool)
{
    ui->preview_window_dock->show();
    emit PreviewWindowIsDisplayed(true);
}

// This is called whenever the user changes the frame or layer selection. It tells the canvas > frame class which
// layer is active in the frame that has been selected.
// TODO: change to Slot/Signal to follow MVC (maybe)
void MainWindow::SelectedLayer()
{
    m_canvas->SelectLayer(ui->layers_list->currentRow()); // (0-indexed, needs to be changed later)
    updateLayerUI(false);
}

// This is called whenever the user changes the frame selection. It tells the canvas > frame class which
// frame is active.
// TODO: change to Slot/Signal to follow MVC (maybe)
void MainWindow::SelectedFrame()
{
    emit SelectFrameSignal(ui->frames_list->currentRow());
   // m_canvas->SelectFrame(ui->frames_list->currentRow());
     updateFrameUI();
}


// Gets the number of layers for the specified frame. frameIndexUI is 0 indexed when passed into this function,
// but is converted later to match up with how it is really stored.
// TODO: change to Slot/Signal to follow MVC (maybe)

int MainWindow::getNumLayers(int frameIndexUI)
{
    return m_canvas->getNumLayers(frameIndexUI);
}



// Updates the UI to show the number of frames currently contained in the canvas vector
void MainWindow::updateFrameUI()
{
    int numFrames = getNumFrames();
    // If we need to update, do so
    if (numFrames != ui->frames_list->count())
    {
        ui->frames_list->clear();
        for (int i = 0; i < numFrames; i++)
        {
            QString frameName("Frame ");
            frameName.append(QString::number(i+1));
            ui->frames_list->addItem(QString(frameName));
        }
        ui->frames_list->setCurrentRow(numFrames-1);
    }
    updateLayerUI(true);
}

// This just adds all the frames to the frames_list UI
void MainWindow::updateLayerUI(bool frameChanged)
{
    // Get the number of layers for the selected frame
    int numLayers = getNumLayers(ui->frames_list->currentRow());
    // If we need to update, do so
    if (numLayers != ui->layers_list->count() || frameChanged)
    {
        // need to store the show bool value before we clear the list
        std::vector<bool> showVal;
        for(int k = 0; k < numLayers; k++)
        {
            showVal.push_back(m_canvas->LayerVisibilityStatus(k+1));
        }

        ui->layers_list->clear();
        for (int i = 0; i < numLayers; i++)
        {
            QString layerName("Layer ");
            layerName.append(QString::number(i + 1));
            ui->layers_list->addItem(layerName);
            ui->layers_list->item(i)->setFlags(ui->layers_list->item(i)->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            if(showVal[i])
            {
            ui->layers_list->item(i)->setCheckState(Qt::Checked); // AND initialize check state
            }
            else
            {
                ui->layers_list->item(i)->setCheckState((Qt::Unchecked));
            }
        }
        ui->layers_list->setCurrentRow(numLayers-1); // Set the selected layer to the last one
    }
}

// Gets the number of frames. It's 0 indexed here,
// but is converted later to match up with how it is really stored.
//TODO: change to Slot/Signal to follow MVC (maybe)
int MainWindow::getNumFrames()
{
    return m_canvas->NumFrames();
}

// Asks the user if they want to save, creates a new canvas of default size 32px by 32px
void MainWindow::CreateNew()
{
    // Save: Yes or No dialog
    QMessageBox msgBox;
    msgBox.setWindowTitle("Save?");
    msgBox.setText("Do you want to save before opening a new sprite?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        SaveProjectFileLastName(); // Calls Matt's method to save
    }
    m_newFormDialog->exec(); // Shows the 'newformdialog' so the user can choose size/color of new sprite
}

void MainWindow::on_colorButton_clicked()
{
    ColorPicker Color(m_canvas->GetBrushColor(), true);
    m_canvas->SetColor(Color.selectColor()); //choose your color and set it as the draw color
    ui->colorButton->setStyleSheet(Color.getRGB() + "; border:1px solid #00ffff"); // set the button to the chosen color
}

void MainWindow::on_colorButtonFill_clicked()
{
    ColorPicker Color(m_canvas->GetFillColor(), true);
    m_canvas->SetColorFill(Color.selectColor()); //choose your color and set it as the draw color
    ui->colorButtonFill->setStyleSheet(Color.getRGB() + "; border:1px solid #00ffff"); // set the button to the chosen color
}

void MainWindow::RetrieveNewFormInfo(int width, int height, QColor color)
{
    // program crashes if the current active frame is not 0 when a new project is opened
    ui->frames_list->setCurrentRow(0);

    BindCanvas(width, height, color);
    delete m_lastFileSave;
    m_lastFileSave = 0;
}

void MainWindow::BindCanvas(int width, int height, QColor color)
{
    delete m_canvas;
    delete m_previewWindow;
    m_previewWindow = 0;
    m_canvas = new Canvas(width, height, color);
    //reset original colors with new window
    ui->colorButton->setStyleSheet("background-color:black; border:1px solid #00ffff");
    ui->colorButtonFill->setStyleSheet("background-color:white; border:1px solid #00ffff");
    ui->brush_size_spinbox->setValue(1);
    BindCanvas();
}

void MainWindow::BindCanvas()
{
    ui->scrollCanvas->setWidget(m_canvas);
    ui->zoom_in->setEnabled(true);
    ui->zoom_out->setEnabled(true);
    connect (ui->zoom_in, SIGNAL(released()), m_canvas, SLOT(ZoomIn()));
    connect (ui->zoom_out, SIGNAL(released()), m_canvas, SLOT(ZoomOut()));
    connect (m_canvas, SIGNAL(ZoomInStatus(bool)), ui->zoom_in, SLOT(setDisabled(bool)));
    connect (m_canvas, SIGNAL(ZoomOutStatus(bool)), ui->zoom_out, SLOT(setDisabled(bool)));
    connect (this, SIGNAL(SetTool(TOOL_ACTIONS)), m_canvas, SLOT(SetTool(TOOL_ACTIONS)));

    // This ensures that the new canvas is bound to the methods to add frames and layers
    connect(this, SIGNAL(AddFrame()), m_canvas, SLOT(AddFrame()));
    connect(this, SIGNAL(AddLayer(int)), m_canvas, SLOT(AddLayerSlot(int)));
    connect(this, SIGNAL(DeleteLayerSignal(int,int)), m_canvas, SLOT(DeleteLayerSlot(int,int)));
    connect(this, SIGNAL(DeleteFrameSignal(int)), m_canvas, SLOT(DeleteFrame(int)));

    //MVC repaint
    connect(this, SIGNAL(RepaintRequest()), m_canvas, SLOT(AcceptRepaintRequest()));

    connect(this, SIGNAL(SelectFrameSignal(int)), m_canvas, SLOT(SelectFrame(int)));
    connect(ui->layers_list,SIGNAL(itemChanged(QListWidgetItem*)), m_canvas, SLOT(UpdateLayerVisibility(QListWidgetItem*)));

    connect(this, SIGNAL(CurrentFrame(int)), m_canvas, SLOT(DuplicateFrame(int)));

    connect(this, SIGNAL(ReverseAnimationSignal()), m_canvas, SLOT(AddReverseAnimation()));

    // This updates the UI with the correct info for frames and layers
    updateFrameUI();

    delete m_previewWindow;
    m_previewWindow = new PreviewWindow(m_canvas, this);
    ui->preview_scroll_area->setBackgroundRole(QPalette::Dark);
    ui->preview_scroll_area->setWidget(m_previewWindow);
    connect(ui->actionOpen_Preview_Window, SIGNAL(triggered(bool)), this, SLOT(showPreview(bool)));
    connect(this, SIGNAL(PreviewWindowIsDisplayed(bool)), m_previewWindow, SLOT(PreviewWindowIsDisplayed(bool)));
    connect(ui->increase_fps_button, SIGNAL(pressed()), m_previewWindow, SLOT(IncreaseFPS()));
    connect(ui->decrease_fps_button, SIGNAL(pressed()), m_previewWindow, SLOT(DecreaseFPS()));
    connect(ui->preview_window_dock, SIGNAL(visibilityChanged(bool)), SLOT(closePreview(bool)));
    connect(m_previewWindow, SIGNAL(updateFPS(int)), this, SLOT(UpdateFPS(int)));

    connect(m_canvas, SIGNAL(RedrawPreview(int)), m_previewWindow, SLOT(UpdateFrame(int)));

    ui->preview_window_dock->show();
    m_previewWindow->UpdateFrame(0);
}

void MainWindow::ExportGIF()
{
#if GIFEXPORT
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("GIF Export"), "",
            tr("GIF (*.gif)"));
    if(fileName != "")
    {
        if(!fileName.endsWith(".gif", Qt::CaseInsensitive))
            fileName.append(".gif");
        StartExportGIF(fileName);
    }
#endif
}

void MainWindow::on_lineButton_clicked()
{
    SetTool(TOOL_ACTIONS::LINE_TOOL);
}

void MainWindow::on_rectButton_clicked()
{
    SetTool(TOOL_ACTIONS::RECT_TOOL);
}

void MainWindow::on_pencilButton_clicked()
{
    SetTool(TOOL_ACTIONS::PENCIL_TOOL);
}

void MainWindow::ExportGIFStart(const QString & fileName)
{
#if GIFEXPORT
    GIFExporter::ExportGifRedo(m_canvas, fileName.toStdString().c_str(), m_previewWindow->getDelay() / 10);
    EndExportGIF();
#endif
}

void MainWindow::ExportGifEnd()
{
    QMessageBox::information(this, "Export GIF", "Export Complete!", QMessageBox::Ok);
}

void MainWindow::SaveProjectFileLastName()
{
    if(m_lastFileSave)
    {
        ProjectFile proj(m_canvas);
        proj.Save(m_lastFileSave->c_str());
    }
    else
        SaveProjectFile();
}

void MainWindow::SaveProjectFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Project"), "",
            tr("Sprite Sheet Project (*.ssp)"));
    if(fileName != "")
    {
        if(!fileName.endsWith(".ssp", Qt::CaseInsensitive))
            fileName.append(".ssp");
        ProjectFile proj(m_canvas);
        if(proj.Save(fileName.toStdString().c_str()))
        {
            delete m_lastFileSave;
            m_lastFileSave = new std::string(fileName.toStdString());
        }
        else
        {
            QMessageBox::critical(this, "Save Project", "Unable to save project!", QMessageBox::Ok);
        }
    }
}

void MainWindow::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Project"), "",
            tr("Sprite Sheet Project (*.ssp)"));

    if(fileName != "")
    {
        if(!fileName.endsWith(".ssp", Qt::CaseInsensitive))
            fileName.append(".ssp");
        try{
            ProjectFile *f = new ProjectFile(fileName.toStdString().c_str());
            Canvas * cnvs = f->GetContents();
            delete m_canvas;
            m_canvas = cnvs;
            BindCanvas();
            delete m_lastFileSave;
            m_lastFileSave = new std::string(fileName.toStdString());
        }
        catch(std::exception)
        {
            QMessageBox::critical(this, "Open Project", "Invalid project file!", QMessageBox::Ok);
        }
        catch(int)
        {
            QMessageBox::critical(this, "Open Project", "Invalid project file!", QMessageBox::Ok);
        }
    }
}


// Emitting AddLayer with the index of the currently selected frame calls the AddLayerSlot method
// in canvas. Then we update the layer UI like always after a layer change.

void MainWindow::on_add_layer_button_clicked()
{
    emit AddLayer(ui->frames_list->currentRow());
    updateLayerUI(false);
}

void MainWindow::on_delete_layer_button_clicked()
{
    emit DeleteLayerSignal(ui->frames_list->currentRow(), ui->layers_list->currentRow());
    updateLayerUI(false);
    emit RepaintRequest();
}

void MainWindow::on_delete_frame_button_clicked()
{
    int currentRow = ui->frames_list->currentRow();
    ui->frames_list->setCurrentRow(0);
    emit DeleteFrameSignal(currentRow);
    updateFrameUI();
}

void MainWindow::on_add_frame_button_clicked()
{
    emit AddFrame();
    updateFrameUI();
}

void MainWindow::on_duplicate_frame_button_clicked()
{
    emit CurrentFrame(ui->frames_list->currentRow());
    updateFrameUI();
}

void MainWindow::on_eraserButton_clicked()
{
     SetTool(TOOL_ACTIONS::ERASER_TOOL);
}


void MainWindow::on_ellipseButton_clicked()
{
    SetTool(TOOL_ACTIONS::ELLIPSE_TOOL);
}

void MainWindow::on_add_reverse_animation_button_clicked()
{
    emit ReverseAnimationSignal();
    updateFrameUI();
}

void MainWindow::on_brush_size_spinbox_valueChanged(int value)
{
    m_canvas->SetBrushSize(value);
}

void MainWindow::on_paintBucketButton_clicked()
{
    m_canvas->SetTool(TOOL_ACTIONS::FILL_TOOL);
}

void MainWindow::on_sprayButton_clicked()
{
    m_canvas->SetTool(TOOL_ACTIONS::SPRAYER_TOOL);
}
