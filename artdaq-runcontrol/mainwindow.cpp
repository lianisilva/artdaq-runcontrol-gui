#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  homePath(QDir::home()),
  localConfigPath(homePath.absolutePath() + "/.config/ARTDAQ/artdaq-runcontrol-gui/"),
  lastRunFileName(localConfigPath.absolutePath() + "/last_run.txt"),
  DAQState(0),
  DAQInterfaceProcess_started(false)
{
  localConfigPath.mkpath(localConfigPath.absolutePath());
  ui->setupUi(this);
  configurateWindow();
  configurateMenuBar();
  initializeFlags();
  initializeButtons();
  initializeButtons();
  ui->lbStatus->setText("");
  connect(ui->bDAQInterface, SIGNAL(clicked(bool)), this, SLOT(bDAQInterfacePressed()));
  connect(&daq_interface, SIGNAL(readyReadStandardOutput()), this, SLOT(DAQInterfaceOutput()));
  connect(&DAQInterface_logwatcher, SIGNAL(fileChanged(QString)), this, SLOT(bDebugPressed()));
  connect(ui->bBelen, SIGNAL(clicked(bool)), this, SLOT(MensajeParaBelen()));
  connect(ui->bDAQCompEtConf, SIGNAL(clicked(bool)), this, SLOT(bListDAQCompsEtConfigs()));
  connect(ui->bLastRunConfig, SIGNAL(clicked(bool)), this, SLOT(bLastRunConfigPressed()));
  connect(ui->bBOOT, SIGNAL(clicked(bool)), this, SLOT(bBOOTPressed()));
  connect(ui->bCONFIG, SIGNAL(clicked(bool)), this, SLOT(bCONFIGPressed()));
  connect(ui->bStart, SIGNAL(clicked(bool)), this, SLOT(bSTARTPressed()));
  connect(&timer, SIGNAL(timeout()), this, SLOT(checkStatus()));
  connect(ui->bStop, SIGNAL(clicked(bool)), this, SLOT(bSTOPPressed()));
  connect(ui->bTerminate, SIGNAL(clicked(bool)), this, SLOT(bTERMINATEPressed()));
  connect(ui->bEndSession, SIGNAL(clicked(bool)), this, SLOT(bEndSessionPressed()));
  connect(ui->bDebug, SIGNAL(clicked(bool)), this, SLOT(bDebugPressed()));
  connect(ui->bListDatabaseRunConfigurations, SIGNAL(clicked(bool)), this, SLOT(bListDatabaseRunConfigurations()));
  connect(ui->checkBoxDatabase, SIGNAL(toggled(bool)), this, SLOT(checkBoxDatabaseChanged()));
  connect(ui->bRun, SIGNAL(clicked(bool)), this, SLOT(bRunPressed()));
  state_diagram.setWindowTitle("DAQInterface State Diagram");
  state_diagram.setFixedSize(state_diagram.geometry().width(), state_diagram.geometry().height());
  state_diagram.show();
}

MainWindow::~MainWindow()
{
  delete ui;
  delete optionsMenu;
  delete exitMenu;
  delete saveConfigMenu;
  delete openConfigMenu;
  delete windowMenu;
  delete Menus;
  delete lvComponentsModel;
  delete lvConfigurationsModel;
  delete lvConfigBOOTModel;
}


void MainWindow::configurateWindow()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  this->setWindowTitle("ARTDAQ RUN CONTROL");
  ui->taDAQInterface->setReadOnly(true);
  this->setDBConfigurationFHICL_dir(env_vars::env.value("HOME") + "/work-db-v4-dir");
  this->originalWindowSize = this->geometry().size();
  this->originalQuadraticMeanConfigurationFontSize =
    (double)qSqrt((qreal)(this->originalWindowSize.height() * this->originalWindowSize.height()
                          + this->originalWindowSize.width() * this->originalWindowSize.width()));
  this->gbDAQInterfaceCommandsPosition = ui->groupBox_DAQInterfaceCommands->pos();
  this->gbDAQInterfaceCommandsSize = ui->groupBox_DAQInterfaceCommands->geometry().size();
  this->gbTransitionCommandsPosition = ui->groupBox_TransitionCommands->pos();
  this->gbTransitionCommandsSize = ui->groupBox_TransitionCommands->geometry().size();
  this->gbDatabasePosition = ui->groupBox_Database->pos();
  this->gbDatabaseSize = ui->groupBox_Database->geometry().size();
  this->gbDAQInterfacePosition = ui->groupBox_DAQInterface->pos();
  this->gbDAQInterfaceSize = ui->groupBox_DAQInterface->geometry().size();
  this->bStartPosition = ui->bStart->pos();
  this->bStartSize = ui->bStart->geometry().size();
  this->bStopPosition = ui->bStop->pos();
  this->bStopSize = ui->bStop->geometry().size();
  this->bPausePosition = ui->bPause->pos();
  this->bPauseSize = ui->bPause->geometry().size();
  this->bTerminatePosition = ui->bTerminate->pos();
  this->bTerminateSize = ui->bTerminate->geometry().size();
  this->bRunPosition = ui->bRun->pos();
  this->bRunSize = ui->bRun->geometry().size();
  this->bListDatabaseRunConfigurationsPosition = ui->bListDatabaseRunConfigurations->pos();
  this->bListDatabaseRunConfigurationsSize = ui->bListDatabaseRunConfigurations->geometry().size();
  this->bDAQCompEtConfPosition = ui->bDAQCompEtConf->pos();
  this->bDAQCompEtConfSize = ui->bDAQCompEtConf->geometry().size();
  this->bLastRunConfigPosition = ui->bLastRunConfig->pos();
  this->bLastRunConfigSize = ui->bLastRunConfig->geometry().size();
  this->bBOOTPosition = ui->bBOOT->pos();
  this->bBOOTSize = ui->bBOOT->geometry().size();
  this->bCONFIGPosition = ui->bCONFIG->pos();
  this->bCONFIGSize = ui->bCONFIG->geometry().size();
  this->bDAQInterfacePosition = ui->bDAQInterface->pos();
  this->bDAQInterfaceSize = ui->bDAQInterface->geometry().size();
  this->bEndSessionPosition = ui->bEndSession->pos();
  this->bEndSessionSize = ui->bEndSession->geometry().size();
  this->taDAQInterfacePosition = ui->taDAQInterface->pos();
  this->taDAQInterfaceSize = ui->taDAQInterface->geometry().size();
  this->checkBoxDatabasePosition = ui->checkBoxDatabase->pos();
  this->checkBoxDatabaseSize = ui->checkBoxDatabase->geometry().size();
  this->lvComponentsPosition = ui->lvComponents->pos();
  this->lvComponentsSize = ui->lvComponents->geometry().size();
  this->lvConfigurationsPosition = ui->lvConfigurations->pos();
  this->lvConfigurationsSize = ui->lvConfigurations->geometry().size();
  this->lvConfigBOOTPosition = ui->lvConfigBOOT->pos();
  this->lvConfigBOOTSize = ui->lvConfigBOOT->geometry().size();
  this->lbConfigurationsPosition = ui->lbConfigurations->pos();
  this->lbConfigurationsSize = ui->lbConfigurations->geometry().size();
  this->lbComponentsPosition = ui->lbComponents->pos();
  this->lbComponentsSize = ui->lbComponents->geometry().size();
  this->lbBOOTConfigPosition = ui->lbBOOTConfig->pos();
  this->lbBOOTConfigSize = ui->lbBOOTConfig->geometry().size();
  this->lbStatusTitlePosition = ui->lbStatusTitle->pos();
  this->lbStatusTitleSize = ui->lbStatusTitle->geometry().size();
  this->lbStatusPosition = ui->lbStatus->pos();
  this->lbStatusSize = ui->lbStatus->geometry().size();
  this->lbMessagesPosition = ui->lbMessages->pos();
  this->lbMessagesSize = ui->lbMessages->geometry().size();
  this->lbConfigurationsFont = ui->lbConfigurations->font();
  this->lbComponentsFont = ui->lbComponents->font();
  this->lbBOOTConfigFont = ui->lbBOOTConfig->font();
  this->lbStatusFont = ui->lbStatus->font();
  this->lbStatusTitleFont = ui->lbStatusTitle->font();
  this->bRunFont = ui->bRun->font();
  this->gbDAQInterfaceCommandsFont = ui->groupBox_DAQInterfaceCommands->font();
  this->gbDAQInterfaceFont = ui->groupBox_DAQInterface->font();
  this->taDAQInterfaceFont = ui->taDAQInterface->font();
  this->lvComponentsFont = ui->lvComponents->font();
  this->lvBOOTConfigFont = ui->lvConfigBOOT->font();
  this->lvConfigurationsFont = ui->lvConfigurations->font();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::configurateMenuBar()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  this->optionsMenu = new QAction("&Options", this);
  this->exitMenu = new QAction("&Exit", this);
  this->openConfigMenu = new QAction("Open Config", this);
  this->saveConfigMenu = new QAction("Save Config", this);
  this->windowMenu = new QAction("&Show state diagram", this);
  this->Menus = new QMenu();
  this->Menus = menuBar()->addMenu("&File");
  this->Menus->addAction(openConfigMenu);
  this->Menus->addAction(saveConfigMenu);
  this->Menus->addAction(exitMenu);
  this->Menus = menuBar()->addMenu("&Tools");
  this->Menus->addAction(optionsMenu);
  this->Menus = menuBar()->addMenu("&Window");
  this->Menus->addAction(windowMenu);
  connect(this->optionsMenu, SIGNAL(triggered(bool)), this, SLOT(openMenuOptionsDialog()));
  connect(this->exitMenu, SIGNAL(triggered(bool)), this, SLOT(closeProgram()));
  connect(this->windowMenu, SIGNAL(triggered(bool)), this, SLOT(showDaqInterfaceStateWindow()));
  connect(this->saveConfigMenu, SIGNAL(triggered(bool)), this, SLOT(saveConfigDialog()));
  connect(this->openConfigMenu, SIGNAL(triggered(bool)), this, SLOT(openConfigDialog()));
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::showDaqInterfaceStateWindow()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  state_diagram.show();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::closeProgram()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  if(DAQInterfaceProcess_started == false){
    QMessageBox::StandardButton msgBox = QMessageBox::question( this, "artdaqRunControl",
                                         tr("Do you really wish to close the program?\n"),
                                         QMessageBox::No | QMessageBox::Yes,
                                         QMessageBox::Yes);
    if (msgBox == QMessageBox::Yes) {
      exit(0);
    }
  }
  else{
    QMessageBox msgBox;
    msgBox.setText("Please end the DAQInteface session before exiting the program");
    msgBox.exec();
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

// This part of the code executes when a closeEvent occurs, i.e. x
// close button on right corner or a alt+F4 is
// pressed. triggered(bool) signal cannot be assigned to this funtion
// because it will generate an sender/receiver incompatibility
// warning, causing misbehavior in the execution.
void MainWindow::closeEvent(QCloseEvent *event)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  if(DAQInterfaceProcess_started == false){
    QMessageBox::StandardButton msgBox = QMessageBox::question( this, "artdaqRunControl",
                                         tr("Do you really wish to close the program?\n"),
                                         QMessageBox::No | QMessageBox::Yes,
                                         QMessageBox::Yes);
    if (msgBox != QMessageBox::Yes) {
      event->ignore();
    }
    else {
      event->accept();
      exit(0);
    }
  }
  else{
    QMessageBox msgBox;
    msgBox.setText("Please end the DAQInteface session before exiting the program");
    msgBox.exec();
    event->ignore();
   }
   qDebug() << "Ending" << Q_FUNC_INFO;
}


void MainWindow::bEndSessionPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QMessageBox msgBox;
  QProcess kill_daqinterface;
  kill_daqinterface.setWorkingDirectory(env_vars::daqInt_wd);
  msgBox.setText("End session");
  msgBox.setInformativeText("Do you really wish to end the session?\n"
                            "The DAQInterface instance in the partition will be destroyed");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  switch (ret) {
  case QMessageBox::Yes:
    kill_daqinterface.execute("kill_daqinterface_on_partition.sh",
                              QStringList() << env_vars::partition_number);
    initializeFlags();
    initializeButtons();
    initializeLV();
    timer.stop();
    status("offline");
    break;
  case QMessageBox::No:
    break;
  default:
    break;
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::initializeFlags()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  flgBOOT = false;
  flgCONFIG = false;
  flgBOOTCONFIG = false;
  flgBOOTED = false;
  flgCONFIGURED = false;
  flgRUNNING = false;
  flgPAUSED = false;
  flgRunPressed = false;
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::initializeButtons()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  ui->bDAQCompEtConf->setEnabled(false);
  ui->bLastRunConfig->setEnabled(false);
  ui->bEndSession->setEnabled(false);
  ui->bBelen->setVisible(false);
  ui->bBelen->setEnabled(false);
  ui->bCONFIG->setEnabled(false);
  ui->bBOOT->setEnabled(false);
  ui->bRun->setEnabled(false);
  ui->bStart->setEnabled(false);
  ui->bStop->setEnabled(false);
  ui->bTerminate->setEnabled(false);
  ui->bPause->setEnabled(false);
  ui->bDAQInterface->setEnabled(true);
  ui->lbStatus->setText("STOPPED");
  ui->checkBoxDatabase->setChecked(false);
  ui->checkBoxDatabase->setEnabled(false);
  ui->bListDatabaseRunConfigurations->setEnabled(false);
  ui->bDebug->setVisible(false);
  ui->bRun->setText("  RUN");

  this->openConfigMenu->setEnabled(false);
  this->saveConfigMenu->setEnabled(false);

  QString imagesDirectory = QCoreApplication::applicationDirPath() + "/../resources/images/";
  QPixmap button_image(imagesDirectory + "start_run.png");
  QIcon ButtonIcon(button_image);
  ui->bRun->setIcon(ButtonIcon);
  ui->bRun->setIconSize(0.9 * button_image.rect().size());
  this->bRunIconSize = 0.9 * button_image.rect().size();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::initializeLV()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  list_comps_selected.clear();
  list_config_selected.clear();
  list_BOOTConfig_selected.clear();
  this->lvComponentsModel = new QStringListModel(this);
  this->lvConfigurationsModel = new QStringListModel(this);
  this->lvConfigBOOTModel = new QStringListModel(this);
  ui->lvConfigBOOT->setModel(this->lvComponentsModel);
  ui->lvComponents->setModel(this->lvConfigurationsModel);
  ui->lvConfigurations->setModel(this->lvConfigBOOTModel);
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::checkStatus()
{
  QString str_status = commDAQInterface.getDAQInterfaceStatus();
  int st = status_map_int.value(str_status);
  if(st >= 1 && st <= 10){
    state_diagram.setOnline();
    // qDebug() << str_status;
    ui->lbStatus->setText(str_status.toUpper());
    status(str_status);
    checkTransitionRunPressed(str_status);
  }
  else{
    ui->taDAQInterface->document()->setPlainText(str_status);
    status("offline");
  }
}

void MainWindow::status(const QString& status)
{
  int st = status_map_int.value(status);
  switch (st) {
  case 1: //stopped
    flgBOOTED = false;
    flgCONFIGURED = false;
    flgRUNNING = false;
    flgPAUSED = false;
    isLVSelected();
    statusTransition();
    state_diagram.setStateDiagramStopped();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedEnabled();
    ui->checkBoxDatabase->setEnabled(true);
    ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lvConfigBOOT->setSelectionMode(QAbstractItemView::SingleSelection);
    if(ui->checkBoxDatabase->isChecked()) {
      ui->bListDatabaseRunConfigurations->setEnabled(true);
      ui->bDAQCompEtConf->setEnabled(false);
      ui->bLastRunConfig->setEnabled(false);
    }
    break;
  case 2: //booted
    flgBOOTED = true;
    flgCONFIGURED = false;
    flgRUNNING = false;
    flgPAUSED = false;
    isLVSelected();
    statusTransition();
    state_diagram.setStateDiagramBooted();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    ui->bTerminate->setEnabled(true);
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lvConfigBOOT->setSelectionMode(QAbstractItemView::SingleSelection);
    break;
  case 3: //ready
    flgBOOTED = true;
    flgCONFIGURED = true;
    flgRUNNING = false;
    flgPAUSED = false;
    isLVSelected();
    statusTransition();
    state_diagram.setStateDiagramReady();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    ui->bTerminate->setEnabled(true);
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 4: // running
    flgRUNNING = true;
    flgPAUSED = false;
    statusTransition();
    state_diagram.setStateDiagramRunning();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 5: // pause
    flgRUNNING = false;
    flgPAUSED = true;
    break;
  case 6: // booting
    state_diagram.setStateDiagramBooting();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    ui->lvComponents->setSelectionMode(QAbstractItemView::NoSelection);
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::NoSelection);
    ui->lvConfigBOOT->setSelectionMode(QAbstractItemView::NoSelection);
    break;
  case 7: // configuring
    state_diagram.setStateDiagramConfiguring();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::NoSelection);
    ui->lvConfigBOOT->setSelectionMode(QAbstractItemView::NoSelection);
    break;
  case 8: // starting
    state_diagram.setStateDiagramStartingRun();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 9: // stopping
    state_diagram.setStateDiagramStoppingRun();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 10: // terminating
    state_diagram.setStateDiagramTerminating();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 99:
    initializeButtons();
    initializeLV();
    timer.stop();
    state_diagram.setOffline();
    state_diagram.setStateDiagramOff();
    state_diagram.setOnlineButtonRed();
    DAQInterfaceProcess_started = false;
    break;
  default:
    break;
  }
}

void MainWindow::setButtonsStoppedEnabled()
{
  ui->bEndSession->setEnabled(true);
  if(!ui->checkBoxDatabase->isEnabled()) {
    ui->bDAQCompEtConf->setEnabled(true);
    ui->bLastRunConfig->setEnabled(true);
  }
}

void MainWindow::setButtonsStoppedDisabled()
{
  ui->bEndSession->setEnabled(false);
  ui->bDAQCompEtConf->setEnabled(false);
  ui->bLastRunConfig->setEnabled(false);
}

void MainWindow::setAllButtonsDisabled()
{
  ui->bEndSession->setEnabled(false);
  ui->bDAQCompEtConf->setEnabled(false);
  ui->bLastRunConfig->setEnabled(false);
  ui->bBOOT->setEnabled(false);
  ui->bCONFIG->setEnabled(false);
  ui->bStart->setEnabled(false);
  ui->bStop->setEnabled(false);
  ui->bTerminate->setEnabled(false);
  ui->bRun->setEnabled(false);
}

void MainWindow::bSTOPPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  commDAQInterface.sendTransitionSTOP();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bTERMINATEPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  commDAQInterface.sendTransitionTERMINATE();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bSTARTPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  saveRunConfig(lastRunFileName);
  commDAQInterface.sendTransitionSTART();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bBOOTPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  commDAQInterface.setDAQInterfaceComponents(list_comps_selected);
  commDAQInterface.sendTransitionBOOT(list_BOOTConfig_selected);
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bCONFIGPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  commDAQInterface.sendTransitionCONFIG(list_config_selected);
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::lvBOOTConfigSelected()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  list_BOOTConfig_selected.clear();
  QStringList list_str;
  QModelIndexList list = ui->lvConfigBOOT->selectionModel()->selectedRows();
  if(list.length() != 0) {
    for(QModelIndex idx : list) {
      list_str = idx.model()->data(idx, Qt::DisplayRole).toString()
        .split(' ', QString::KeepEmptyParts);
      QString s_ = env_vars::daqInt_user_dir + "/" + list_str.first();
      list_BOOTConfig_selected.append(s_);
      list_str.clear();
    }
    flgBOOTCONFIG = true;
  }
  else {
    QStringListModel* unselectedListModel = (QStringListModel*)ui->lvConfigBOOT->model();
    if(unselectedListModel->stringList().length() == 1) {
      list_BOOTConfig_selected.clear();
      QString s_ = env_vars::daqInt_user_dir + "/" + unselectedListModel->stringList().first();
      list_BOOTConfig_selected.append(s_);
      flgBOOTCONFIG = true;
      flgBOOT = true;
    }
    else {
      flgBOOTCONFIG = false;
      flgBOOT = true;
    }
  }
  isLVSelected();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::lvComponentsSelected()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  if(!ui->checkBoxDatabase->isChecked()) {
    list_comps_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvComponents->selectionModel()->selectedRows();
    if(list.length() != 0) {
      for(QModelIndex idx : list) {
        list_str = idx.model()->data(idx, Qt::DisplayRole).toString()
          .split(' ', QString::KeepEmptyParts);
        list_comps_selected.append(list_str.first());
        list_str.clear();
      }
      flgBOOT = true;
    }
    else {
      flgBOOT = false;
    }
    isLVSelected();
  }
  else {
    list_comps_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvComponents->selectionModel()->selectedRows();
    if(list.length() != 0) {
      for(QModelIndex idx : list) {
        list_str = idx.model()->data(idx, Qt::DisplayRole).toString()
          .split(' ', QString::KeepEmptyParts);
        list_comps_selected.append(list_str.first());
        list_str.clear();
      }
      flgBOOT = true;
    }
    else {
      QStringListModel* unselectedListModel = (QStringListModel*)ui->lvComponents->model();
      list_comps_selected = unselectedListModel->stringList();
      flgBOOT = true;
    }
    isLVSelected();
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::lvConfigurationsSelected()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  if(DAQState == 3) {
    flgCONFIG = true;
    isLVSelected();
  }
  else if(!ui->checkBoxDatabase->isChecked()) {
    list_config_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvConfigurations->selectionModel()->selectedRows();
    if(list.length() != 0) {
      for(QModelIndex idx : list) {
        list_str = idx.model()->data(idx, Qt::DisplayRole).toString()
          .split(' ', QString::KeepEmptyParts);
        list_config_selected.append(list_str.first());
        list_str.clear();
      }
      flgCONFIG = true;
    }
    else {
      QStringListModel* unselectedListModel = (QStringListModel*)ui->lvConfigurations->model();
      if(unselectedListModel->stringList().length() == 1) {
        list_config_selected = unselectedListModel->stringList();
        flgCONFIG = true;
      }
      else {
        flgCONFIG = false;
      }
    }
    isLVSelected();
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::statusTransition()
{
  if(flgRUNNING) {
    ui->bStart->setEnabled(false);
    ui->bStop->setEnabled(true);
  }
  else {
    ui->bStop->setEnabled(false);
  }
}

void MainWindow::isLVSelected()
{
  if(flgBOOT && flgCONFIG && flgBOOTCONFIG && !flgBOOTED && !flgCONFIGURED) {
    ui->bBOOT->setEnabled(true);
    ui->bCONFIG->setEnabled(false);
    ui->bRun->setEnabled(true);
    ui->bTerminate->setEnabled(false);
    ui->bStart->setEnabled(false);
    // qDebug()<<"selected: 1";
  }
  else if(flgBOOT && flgCONFIG && flgBOOTCONFIG && !flgBOOTED) {
    ui->bBOOT->setEnabled(true);
    // qDebug()<<"selected: 2";
  }
  else if(!flgBOOT || !flgBOOTCONFIG || !flgCONFIG) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bRun->setEnabled(false);
    // qDebug()<<"selected: 3";
  }
  else if(!flgCONFIG && flgBOOTED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bRun->setEnabled(false);
    // qDebug()<<"selected: 4";
  }
  else if(flgBOOTED && flgCONFIG && !flgCONFIGURED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(true);
    ui->bRun->setEnabled(false);
    ui->bTerminate->setEnabled(true);
    // qDebug()<<"selected: 5";
  }
  else if(flgBOOTED && flgCONFIGURED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bRun->setEnabled(false);
    ui->bStart->setEnabled(true);
    // qDebug()<<"selected: 6";
  }
  else if(flgBOOTED && !flgCONFIGURED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(true);
    ui->bRun->setEnabled(false);
    // qDebug()<<"selected: 7";
  }
}

void MainWindow::setButtonsDAQInterfaceInitialized(bool started)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  if(started) {
    ui->bDAQInterface->setEnabled(false);
    ui->bDAQCompEtConf->setEnabled(true);
    ui->bLastRunConfig->setEnabled(true);
    ui->bEndSession->setEnabled(true);
    ui->checkBoxDatabase->setEnabled(true);
    this->openConfigMenu->setEnabled(true);
    this->saveConfigMenu->setEnabled(true);
    timer.start(1000);
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bDAQInterfacePressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  ui->bDAQInterface->setEnabled(false);
  daq_interface.setWorkingDirectory(env_vars::daqInt_wd);

  if (env_vars::daqInt_user_sourcefile != "EMPTY") {
    qDebug() << "env_vars::daqInt_user_sourcefile: " << env_vars::daqInt_user_sourcefile;
    daq_interface.start("./bin/DAQInterface.sh");
    DAQInterfaceProcess_started = true;
    DAQInterface_PID = daq_interface.processId();
    setButtonsDAQInterfaceInitialized(DAQInterfaceProcess_started);

    state_diagram.setLCDPartitionNumber(env_vars::partition_number.toInt());
    state_diagram.setLCDPortNumber(env_vars::rpc_port.toInt());
    checkBoxDatabaseChanged();
  }
  else {
    qCritical("Source script hasn't been sourced.\n"
              "DAQINTERFACE_USER_SOURCEFILE: %s",
              qUtf8Printable(env_vars::daqInt_user_sourcefile));
    QCoreApplication::exit(1);
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
  return;
}

void MainWindow::DAQInterfaceOutput()
{
  //qDebug() << "Starting" << Q_FUNC_INFO;
  QByteArray daq_byte_array = daq_interface.readAllStandardOutput();
  QTextCodec* codec = QTextCodec::codecForName("UTF-8");
  daq_string = codec->codecForMib(106)->toUnicode(daq_byte_array);
  daqInterfaceTextAreaLog = daqInterfaceTextAreaLog + daq_string;
  ui->taDAQInterface->document()->setPlainText(daqInterfaceTextAreaLog);
  QScrollBar* scroll = ui->taDAQInterface->verticalScrollBar();
  scroll->setValue(scroll->maximum());
  // qDebug() << "Inside " << __func__ << " , daq_string: " << daq_string << "\n";
  switch (DAQState) {
  case 1:
    populateLVComps(daq_string);
    this->listDAQConfigs();
    break;
  case 2:
    populateLVConfigs(daq_string);
    QThread::msleep(100);
    ui->bDAQCompEtConf->setEnabled(true);
    break;
  case 3:
    populateLVConfigurationsFromDatabase();
    populateLVComponentsFromDatabase();
    populateLVBOOTConfigurationsFromDatabase();
    this->lvComponentsSelected();
    this->lvConfigurationsSelected();
    this->lvBOOTConfigSelected();
    DAQState = 0;
  default:
    break;
  }
  //qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::retrieveConfigFromFile(const QString &runFileName)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QFile f(runFileName);
  if(!f.open(QFile::ReadOnly |
             QFile::Text)) {
    qInfo() << " Could not open file "
               << runFileName << " for writing";
    return;
  }
  QTextStream in(&f);
  unsigned checks = 0;
  QStringList usf = (in.readLine()).split(" ");
  if(usf[0] == "DAQINTERFACE_USER_SOURCEFILE:") {
    if(usf[1] == env_vars::daqInt_user_sourcefile) checks++;
  }
  if(checks == 0){
    qWarning()
      << "Current DAQINTERFACE_USER_SOURCEFILE: " << env_vars::daqInt_user_sourcefile
      << ",\n\tdoesn't match the one stored: " << usf[1]
      << ".\n\tCan't use last run Components and Configs.";
    return;
  }
  QStringList sl;
  sl = (in.readLine()).split(" ");
  if(sl[0] == "components:") {
    sl.removeFirst();
    list_comps_selected = sl;
    list_comps_selected.sort();
    QStringListModel* model = (QStringListModel*)ui->lvComponents->model();
    model->setStringList(list_comps_selected);
    ui->lvComponents->setSelectionMode(QAbstractItemView::NoSelection);
    checks++;
    qInfo() << "components: " << list_comps_selected;
  }
  sl = (in.readLine()).split(" ");
  if(sl[0] == "configs:") {
    sl.removeFirst();
    list_config_selected = sl;
    QStringListModel* model = (QStringListModel*)ui->lvConfigurations->model();
    model->setStringList(list_config_selected);
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::NoSelection);
    checks++;
    qInfo() << "configs: " << list_config_selected;
  }
  sl = (in.readLine()).split(" ");
  if(sl[0] == "boot_configs:") {
    sl.removeFirst();
    list_BOOTConfig_selected = sl;
    QFileInfo boot_fInfo(list_BOOTConfig_selected[0]);
    QStringList boot_fname(boot_fInfo.fileName());
    QStringListModel* model = (QStringListModel*)ui->lvConfigBOOT->model();
    ui->lvConfigBOOT->setSelectionMode(QAbstractItemView::NoSelection);
    model->setStringList(boot_fname);
    checks++;
    qInfo() << "boot_configs: " << list_BOOTConfig_selected;
  }
  if(checks != 4){
    qWarning()
      << "Didn't pass all the checks, so can't use Components and Configs from "
      << runFileName;
    return;
  }
  flgCONFIG = true;
  flgBOOTCONFIG = true;
  flgBOOT = true;
  ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
  ui->lvComponents->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(ui->lvComponents->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvComponentsSelected()));
  connect(ui->lvConfigurations->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvConfigurationsSelected()));
  connect(ui->lvConfigBOOT->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvBOOTConfigSelected()));
  ui->lvComponents->selectAll();
  f.close();
  qDebug() << "Ending" << Q_FUNC_INFO;
}
void MainWindow::bLastRunConfigPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  this->retrieveConfigFromFile(lastRunFileName);
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::saveRunConfig(const QString& runFileName)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  qDebug() << "Saving run info to file: " << runFileName << "\n";
  QFile f(runFileName);
  if(!f.open(QFile::WriteOnly |
             QFile::Text)) {
    qWarning() << " Could not open file "
               << runFileName << " for writing";
    return;
  }
  QTextStream out(&f);
  list_comps_selected.sort();
  out << "DAQINTERFACE_USER_SOURCEFILE: " << env_vars::daqInt_user_sourcefile << "\n";
  out << "components: " << list_comps_selected.join(" ") << "\n";
  out << "configs: " << list_config_selected.join(" ") << "\n";
  out << "boot_configs: " << list_BOOTConfig_selected.join(" ") << "\n";
  f.flush();
  f.close();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bListDAQCompsEtConfigs()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  ui->bDAQCompEtConf->setEnabled(false);
  this->listDAQComps();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::listDAQComps()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  list_comps_selected.clear();
  list_config_selected.clear();
  list_BOOTConfig_selected.clear();
  commDAQInterface.listDAQInterfaceComponents();
  DAQState = 1;
  QThread::msleep(100);
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::populateLVComps(const QString& di_comps_output)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QStringList list = di_comps_output.split('\n', QString::SkipEmptyParts);
  list.removeFirst();
  QStringListModel* model = (QStringListModel*)ui->lvComponents->model();
  model->setStringList(list);
  ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
  connect(ui->lvComponents->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvComponentsSelected()));
  DAQState = 0;
  this->lvComponentsSelected();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::listDAQConfigs()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  commDAQInterface.listDAQInterfaceConfigs();
  DAQState = 2;
  QRegExp reg("*.txt");
  reg.setPatternSyntax(QRegExp::Wildcard);
  QDirIterator dirIt(env_vars::daqInt_user_dir);
  QString s_;
  QStringList list_str, list_config;
  while(dirIt.hasNext()) {
    s_ = dirIt.next();
    if(reg.exactMatch(s_)) {
      // qDebug() << "config file " << s_;
      list_str = s_.split('/', QString::SkipEmptyParts);
      list_config.append(list_str.last());
    }
    else {
    }
  }

  QStringListModel* model = (QStringListModel*)ui->lvConfigBOOT->model();
  model->setStringList(list_config);
  ui->lvConfigBOOT->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(ui->lvConfigBOOT->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvBOOTConfigSelected()));
  QThread::msleep(100);
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::populateLVConfigs(const QString& di_configs_output)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QStringListModel* model = (QStringListModel*)ui->lvConfigurations->model();
  QStringList list = di_configs_output.split("\n\n", QString::SkipEmptyParts);
  const QString& list_config = list.at(0);
  list = list_config.split('\n');
  //qDebug()<<list;
  list.removeFirst();
  list.removeFirst();
  model->setStringList(list);
  connect(ui->lvConfigurations->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvConfigurationsSelected()));
  DAQState = 0;
  this->lvConfigurationsSelected();
  this->lvBOOTConfigSelected();
  this->lvComponentsSelected();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::bRunPressed()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  flgRunPressed = true;
  runConfigSignalIssued = false;
  runStartSignalIssued = false;
  saveRunConfig(lastRunFileName);
  this->bBOOTPressed();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::checkTransitionRunPressed(const QString& status)
{
  int st = status_map_int.value(status);
  if(flgRunPressed) {
    switch(st) {
    case 1: //stopped
      flgRunPressed = false;
      runConfigSignalIssued = false;
      runStartSignalIssued = false;
      break;
    case 2: //booted
      if(!runConfigSignalIssued) {
        this->bCONFIGPressed();
      }
      else {
        //TODO: Add error message
      }
      break;
    case 3: //ready
      if(!runStartSignalIssued) {
        this->bSTARTPressed();
      }
      else {
        //TODO: Add error message
      }
      runConfigSignalIssued = false;
      break;
    case 4: // running
      runStartSignalIssued = false;
      break;
    case 5: // pause

      break;
    case 6: // booting

      break;
    case 7: // configuring
      runConfigSignalIssued = true;
      break;
    case 8: // starting
      runStartSignalIssued = true;
      break;
    case 9: // stopping
      flgRunPressed = false;
      break;
    case 10: // terminating
      flgRunPressed = false;
      break;
    case 99:

      break;
    default:
      break;
    }
  }
}

void MainWindow::bDebugPressed()
{
  qDebug() << "Debug";
  // db_dialog *dialogConftoolImport = new db_dialog(this);
  // dialogConftoolImport->setWindowTitle("Import configuration from Database");
  // int result = dialogConftoolImport->exec();
  // if(result == QDialog::Accepted){

  // }
  // else if(result == QDialog::Rejected){}
}

void MainWindow::bListDatabaseRunConfigurations()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  db_dialog *dbDialog = new db_dialog(this);
  dbDialog->setWindowTitle("List Of Available Run Configurations");
  int result = dbDialog->exec();
  if(result == QDialog::Accepted) {

    // TODO: do something along the lines of the selected answer here:
    // https://stackoverflow.com/questions/9194799/how-to-return-data-from-qdialog
    dbSelectedConfig = dbDialog->getSelectedDBConfig();
    qDebug() << "dialog_selected_config: " << dbSelectedConfig;
    commDAQInterface.listDAQInterfaceComponents();
    DAQState = 3;
  }
  else if(result == QDialog::Rejected) {}
  delete dbDialog;
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::checkBoxDatabaseChanged()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  bool checked = ui->checkBoxDatabase->isChecked();
  if(checked) {
    ui->bListDatabaseRunConfigurations->setEnabled(true);
    ui->bDAQCompEtConf->setEnabled(false);
    ui->bLastRunConfig->setEnabled(false);
    initializeLV();
    flgBOOTCONFIG = false;
  }
  else {
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    ui->bDAQCompEtConf->setEnabled(true);
    ui->bLastRunConfig->setEnabled(true);
    initializeLV();
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lvConfigurations->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->lvComponents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    flgBOOTCONFIG = false;
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::populateLVComponentsFromDatabase()
{
  qDebug() << "Starting function " << Q_FUNC_INFO;
  flgBOOTCONFIG = false;

  QString config_name = dbSelectedConfig.first;
  config_name.chop(5); // to remove the numbers // TODO: find a better way
  QString selectedDBConfig_dir = dbSelectedConfig.second + "/" + config_name;
  QDirIterator dirIt(selectedDBConfig_dir, QDir::AllEntries | QDir::NoDotAndDotDot,
                     QDirIterator::Subdirectories);

  QStringList componentlist = daq_string.split('\n', QString::SkipEmptyParts);
  QStringList lvComponentsList;
  for(QString component : componentlist) {
    QStringList component_ = component.split(' ', QString::SkipEmptyParts);
    componentlist.replace(componentlist.indexOf(component), component_.at(0));
  }
  componentlist.removeFirst();
  //  this section of the code compares know_components_list and the components in the DB
  while (dirIt.hasNext()) {
    QString fileName = dirIt.next();
    QStringList fileName_ = fileName.split('/', QString::KeepEmptyParts);
    fileName = fileName_.last();
    fileName.replace(".fcl","");
    qDebug() << "fileName: " << fileName;
    for(QString component : componentlist) {
      qDebug() << "component: " << component;
      if(component == fileName){
        // TODO: if relevant, also remove component from componentList
        lvComponentsList.append(component);
        break;
      }
    }
  }

  lvComponentsList.sort();
  QStringListModel* model = (QStringListModel*)ui->lvComponents->model();
  model->setStringList(lvComponentsList);
  ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
  ui->lvComponents->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(ui->lvComponents->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvComponentsSelected()));
  list_comps_selected = lvComponentsList;
  ui->lvComponents->selectAll();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::populateLVConfigurationsFromDatabase()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QStringList lvConfigurationsList;
  lvConfigurationsList.append(dbSelectedConfig.first);
  QStringListModel* model = (QStringListModel*)ui->lvConfigurations->model();
  model->setStringList(lvConfigurationsList);
  ui->lvConfigurations->setSelectionMode(QAbstractItemView::NoSelection);
  ui->lvConfigurations->setEditTriggers(QAbstractItemView::NoEditTriggers);
  list_config_selected = lvConfigurationsList;
  ui->lvConfigurations->selectAll();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::populateLVBOOTConfigurationsFromDatabase()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QRegExp reg("*.txt");
  reg.setPatternSyntax(QRegExp::Wildcard);
  QDirIterator dirIt(env_vars::daqInt_user_dir);
  QString s_;
  QStringList list_str, list_config;
  bool foundMatch = false;
  while(dirIt.hasNext()) {
    s_ = dirIt.next();
    if(reg.exactMatch(s_)) {
      // qDebug() << "config file "<< str;
      list_str = s_.split('/', QString::SkipEmptyParts);
      // qDebug() << list_str.last();
      list_config.append(list_str.last());
      foundMatch = true;
    }
  }
  if (foundMatch) {
    QStringListModel* model = (QStringListModel*)ui->lvConfigBOOT->model();
    model->setStringList(list_config);
    connect(ui->lvConfigBOOT->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(lvBOOTConfigSelected()));
    ui->lvConfigBOOT->setEditTriggers(QAbstractItemView::NoEditTriggers);
  }
  else qInfo() << "No common config files found.";
  qDebug() << "Ending" << Q_FUNC_INFO;
}

// QString MainWindow::getDBConfigurationFHICL_dir() const
// {
//   qDebug() << "Starting" << Q_FUNC_INFO;
//   return DBConfigurationFHICL_dir;
//   qDebug() << "Ending" << Q_FUNC_INFO;
// }

void MainWindow::setDBConfigurationFHICL_dir(const QString& dir)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  DBConfigurationFHICL_dir = dir;
  qDebug() << "Ending" << Q_FUNC_INFO;
}

// QProcessEnvironment MainWindow::getQProcessEnvironment()
// {
//   qDebug() << "Starting" << Q_FUNC_INFO;
//   return env_vars::env;
//   qDebug() << "Ending" << Q_FUNC_INFO;
// }

void MainWindow::resizeEvent(QResizeEvent* event)
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QMainWindow::resizeEvent(event);
  this->resizeWindow();
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::resizeWindow()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  QSize windowSize = this->geometry().size();
  int windowHeigth = windowSize.height();
  int windowWidth = windowSize.width();
  ui->groupBox_DAQInterfaceCommands->move((int)(windowWidth * this->gbDAQInterfaceCommandsPosition.x() / this->originalWindowSize.width()), ui->groupBox_DAQInterfaceCommands->pos().y());
  ui->groupBox_DAQInterfaceCommands->move(ui->groupBox_DAQInterfaceCommands->pos().x(), (int)(windowHeigth * this->gbDAQInterfaceCommandsPosition.y() / this->originalWindowSize.height()));
  ui->groupBox_DAQInterfaceCommands->resize(ui->groupBox_DAQInterfaceCommands->geometry().size().width(), windowHeigth * this->gbDAQInterfaceCommandsSize.height() / this->originalWindowSize.height());
  ui->groupBox_DAQInterfaceCommands->resize(windowWidth * this->gbDAQInterfaceCommandsSize.width() / this->originalWindowSize.width(), ui->groupBox_DAQInterfaceCommands->geometry().size().height());

  ui->groupBox_TransitionCommands->move((int)(windowWidth * this->gbTransitionCommandsPosition.x() / this->originalWindowSize.width()), ui->groupBox_TransitionCommands->pos().y());
  ui->groupBox_TransitionCommands->move(ui->groupBox_TransitionCommands->pos().x(), (int)(windowHeigth * this->gbTransitionCommandsPosition.y() / this->originalWindowSize.height()));
  ui->groupBox_TransitionCommands->resize(ui->groupBox_TransitionCommands->geometry().size().width(), windowHeigth * this->gbTransitionCommandsSize.height() / this->originalWindowSize.height());
  ui->groupBox_TransitionCommands->resize(windowWidth * this->gbTransitionCommandsSize.width() / this->originalWindowSize.width(), ui->groupBox_TransitionCommands->geometry().size().height());

  ui->groupBox_Database->move((int)(windowWidth * this->gbDatabasePosition.x() / this->originalWindowSize.width()), ui->groupBox_Database->pos().y());
  ui->groupBox_Database->move(ui->groupBox_Database->pos().x(), (int)(windowHeigth * this->gbDatabasePosition.y() / this->originalWindowSize.height()));
  ui->groupBox_Database->resize(ui->groupBox_Database->geometry().size().width(), windowHeigth * this->gbDatabaseSize.height() / this->originalWindowSize.height());
  ui->groupBox_Database->resize(windowWidth * this->gbDatabaseSize.width() / this->originalWindowSize.width(), ui->groupBox_Database->geometry().size().height());

  ui->groupBox_DAQInterface->move((int)(windowWidth * this->gbDAQInterfacePosition.x() / this->originalWindowSize.width()), ui->groupBox_DAQInterface->pos().y());
  ui->groupBox_DAQInterface->move(ui->groupBox_DAQInterface->pos().x(), (int)(windowHeigth * this->gbDAQInterfacePosition.y() / this->originalWindowSize.height()));
  ui->groupBox_DAQInterface->resize(ui->groupBox_DAQInterface->geometry().size().width(), windowHeigth * this->gbDAQInterfaceSize.height() / this->originalWindowSize.height());
  ui->groupBox_DAQInterface->resize(windowWidth * this->gbDAQInterfaceSize.width() / this->originalWindowSize.width(), ui->groupBox_DAQInterface->geometry().size().height());

  ui->bStart->move((int)(windowWidth * this->bStartPosition.x() / this->originalWindowSize.width()), ui->bStart->pos().y());
  ui->bStart->move(ui->bStart->pos().x(), (int)(windowHeigth * this->bStartPosition.y() / this->originalWindowSize.height()));
  ui->bStart->resize(ui->bStart->geometry().size().width(), windowHeigth * this->bStartSize.height() / this->originalWindowSize.height());
  ui->bStart->resize(windowWidth * this->bStartSize.width() / this->originalWindowSize.width(), ui->bStart->geometry().size().height());

  ui->bStop->move((int)(windowWidth * this->bStopPosition.x() / this->originalWindowSize.width()), ui->bStop->pos().y());
  ui->bStop->move(ui->bStop->pos().x(), (int)(windowHeigth * this->bStopPosition.y() / this->originalWindowSize.height()));
  ui->bStop->resize(ui->bStop->geometry().size().width(), windowHeigth * this->bStopSize.height() / this->originalWindowSize.height());
  ui->bStop->resize(windowWidth * this->bStopSize.width() / this->originalWindowSize.width(), ui->bStop->geometry().size().height());

  ui->bPause->move((int)(windowWidth * this->bPausePosition.x() / this->originalWindowSize.width()), ui->bPause->pos().y());
  ui->bPause->move(ui->bPause->pos().x(), (int)(windowHeigth * this->bPausePosition.y() / this->originalWindowSize.height()));
  ui->bPause->resize(ui->bPause->geometry().size().width(), windowHeigth * this->bPauseSize.height() / this->originalWindowSize.height());
  ui->bPause->resize(windowWidth * this->bPauseSize.width() / this->originalWindowSize.width(), ui->bPause->geometry().size().height());

  ui->bTerminate->move((int)(windowWidth * this->bTerminatePosition.x() / this->originalWindowSize.width()), ui->bTerminate->pos().y());
  ui->bTerminate->move(ui->bTerminate->pos().x(), (int)(windowHeigth * this->bTerminatePosition.y() / this->originalWindowSize.height()));
  ui->bTerminate->resize(ui->bTerminate->geometry().size().width(), windowHeigth * this->bTerminateSize.height() / this->originalWindowSize.height());
  ui->bTerminate->resize(windowWidth * this->bTerminateSize.width() / this->originalWindowSize.width(), ui->bTerminate->geometry().size().height());

  ui->bRun->move((int)(windowWidth * this->bRunPosition.x() / this->originalWindowSize.width()), ui->bRun->pos().y());
  ui->bRun->move(ui->bRun->pos().x(), (int)(windowHeigth * this->bRunPosition.y() / this->originalWindowSize.height()));
  ui->bRun->resize(ui->bRun->geometry().size().width(), windowHeigth * this->bRunSize.height() / this->originalWindowSize.height());
  ui->bRun->resize(windowWidth * this->bRunSize.width() / this->originalWindowSize.width(), ui->bRun->geometry().size().height());

  ui->bListDatabaseRunConfigurations->move((int)(windowWidth * this->bListDatabaseRunConfigurationsPosition.x() / this->originalWindowSize.width()), ui->bListDatabaseRunConfigurations->pos().y());
  ui->bListDatabaseRunConfigurations->move(ui->bListDatabaseRunConfigurations->pos().x(), (int)(windowHeigth * this->bListDatabaseRunConfigurationsPosition.y() / this->originalWindowSize.height()));
  ui->bListDatabaseRunConfigurations->resize(ui->bListDatabaseRunConfigurations->geometry().size().width(), windowHeigth * this->bListDatabaseRunConfigurationsSize.height() / this->originalWindowSize.height());
  ui->bListDatabaseRunConfigurations->resize(windowWidth * this->bListDatabaseRunConfigurationsSize.width() / this->originalWindowSize.width(), ui->bListDatabaseRunConfigurations->geometry().size().height());

  ui->bDAQCompEtConf->move((int)(windowWidth * this->bDAQCompEtConfPosition.x() / this->originalWindowSize.width()), ui->bDAQCompEtConf->pos().y());
  ui->bDAQCompEtConf->move(ui->bDAQCompEtConf->pos().x(), (int)(windowHeigth * this->bDAQCompEtConfPosition.y() / this->originalWindowSize.height()));
  ui->bDAQCompEtConf->resize(ui->bDAQCompEtConf->geometry().size().width(), windowHeigth * this->bDAQCompEtConfSize.height() / this->originalWindowSize.height());
  ui->bDAQCompEtConf->resize(windowWidth * this->bDAQCompEtConfSize.width() / this->originalWindowSize.width(), ui->bDAQCompEtConf->geometry().size().height());

  ui->bLastRunConfig->move((int)(windowWidth * this->bLastRunConfigPosition.x() / this->originalWindowSize.width()), ui->bLastRunConfig->pos().y());
  ui->bLastRunConfig->move(ui->bLastRunConfig->pos().x(), (int)(windowHeigth * this->bLastRunConfigPosition.y() / this->originalWindowSize.height()));
  ui->bLastRunConfig->resize(ui->bLastRunConfig->geometry().size().width(), windowHeigth * this->bLastRunConfigSize.height() / this->originalWindowSize.height());
  ui->bLastRunConfig->resize(windowWidth * this->bLastRunConfigSize.width() / this->originalWindowSize.width(), ui->bLastRunConfig->geometry().size().height());

  ui->bBOOT->move((int)(windowWidth * this->bBOOTPosition.x() / this->originalWindowSize.width()), ui->bBOOT->pos().y());
  ui->bBOOT->move(ui->bBOOT->pos().x(), (int)(windowHeigth * this->bBOOTPosition.y() / this->originalWindowSize.height()));
  ui->bBOOT->resize(ui->bBOOT->geometry().size().width(), windowHeigth * this->bBOOTSize.height() / this->originalWindowSize.height());
  ui->bBOOT->resize(windowWidth * this->bBOOTSize.width() / this->originalWindowSize.width(), ui->bBOOT->geometry().size().height());

  ui->bCONFIG->move((int)(windowWidth * this->bCONFIGPosition.x() / this->originalWindowSize.width()), ui->bCONFIG->pos().y());
  ui->bCONFIG->move(ui->bCONFIG->pos().x(), (int)(windowHeigth * this->bCONFIGPosition.y() / this->originalWindowSize.height()));
  ui->bCONFIG->resize(ui->bCONFIG->geometry().size().width(), windowHeigth * this->bCONFIGSize.height() / this->originalWindowSize.height());
  ui->bCONFIG->resize(windowWidth * this->bCONFIGSize.width() / this->originalWindowSize.width(), ui->bCONFIG->geometry().size().height());

  ui->bDAQInterface->move((int)(windowWidth * this->bDAQInterfacePosition.x() / this->originalWindowSize.width()), ui->bDAQInterface->pos().y());
  ui->bDAQInterface->move(ui->bDAQInterface->pos().x(), (int)(windowHeigth * this->bDAQInterfacePosition.y() / this->originalWindowSize.height()));
  ui->bDAQInterface->resize(ui->bDAQInterface->geometry().size().width(), windowHeigth * this->bDAQInterfaceSize.height() / this->originalWindowSize.height());
  ui->bDAQInterface->resize(windowWidth * this->bDAQInterfaceSize.width() / this->originalWindowSize.width(), ui->bDAQInterface->geometry().size().height());

  ui->bEndSession->move((int)(windowWidth * this->bEndSessionPosition.x() / this->originalWindowSize.width()), ui->bEndSession->pos().y());
  ui->bEndSession->move(ui->bEndSession->pos().x(), (int)(windowHeigth * this->bEndSessionPosition.y() / this->originalWindowSize.height()));
  ui->bEndSession->resize(ui->bEndSession->geometry().size().width(), windowHeigth * this->bEndSessionSize.height() / this->originalWindowSize.height());
  ui->bEndSession->resize(windowWidth * this->bEndSessionSize.width() / this->originalWindowSize.width(), ui->bEndSession->geometry().size().height());

  ui->taDAQInterface->move((int)(windowWidth * this->taDAQInterfacePosition.x() / this->originalWindowSize.width()), ui->taDAQInterface->pos().y());
  ui->taDAQInterface->move(ui->taDAQInterface->pos().x(), (int)(windowHeigth * this->taDAQInterfacePosition.y() / this->originalWindowSize.height()));
  ui->taDAQInterface->resize(ui->taDAQInterface->geometry().size().width(), windowHeigth * this->taDAQInterfaceSize.height() / this->originalWindowSize.height());
  ui->taDAQInterface->resize(windowWidth * this->taDAQInterfaceSize.width() / this->originalWindowSize.width(), ui->taDAQInterface->geometry().size().height());

  ui->lbConfigurations->move((int)(windowWidth * this->lbConfigurationsPosition.x() / this->originalWindowSize.width()), ui->lbConfigurations->pos().y());
  ui->lbConfigurations->move(ui->lbConfigurations->pos().x(), (int)(windowHeigth * this->lbConfigurationsPosition.y() / this->originalWindowSize.height()));
  ui->lbConfigurations->resize(ui->lbConfigurations->geometry().size().width(), windowHeigth * this->lbConfigurationsSize.height() / this->originalWindowSize.height());
  ui->lbConfigurations->resize(windowWidth * this->lbConfigurationsSize.width() / this->originalWindowSize.width(), ui->lbConfigurations->geometry().size().height());

  ui->lbComponents->move((int)(windowWidth * this->lbComponentsPosition.x() / this->originalWindowSize.width()), ui->lbComponents->pos().y());
  ui->lbComponents->move(ui->lbComponents->pos().x(), (int)(windowHeigth * this->lbComponentsPosition.y() / this->originalWindowSize.height()));
  ui->lbComponents->resize(ui->lbComponents->geometry().size().width(), windowHeigth * this->lbComponentsSize.height() / this->originalWindowSize.height());
  ui->lbComponents->resize(windowWidth * this->lbComponentsSize.width() / this->originalWindowSize.width(), ui->lbComponents->geometry().size().height());

  ui->lbBOOTConfig->move((int)(windowWidth * this->lbBOOTConfigPosition.x() / this->originalWindowSize.width()), ui->lbBOOTConfig->pos().y());
  ui->lbBOOTConfig->move(ui->lbBOOTConfig->pos().x(), (int)(windowHeigth * this->lbBOOTConfigPosition.y() / this->originalWindowSize.height()));
  ui->lbBOOTConfig->resize(ui->lbBOOTConfig->geometry().size().width(), windowHeigth * this->lbBOOTConfigSize.height() / this->originalWindowSize.height());
  ui->lbBOOTConfig->resize(windowWidth * this->lbBOOTConfigSize.width() / this->originalWindowSize.width(), ui->lbBOOTConfig->geometry().size().height());

  ui->lbStatus->move((int)(windowWidth * this->lbStatusPosition.x() / this->originalWindowSize.width()), ui->lbStatus->pos().y());
  ui->lbStatus->move(ui->lbStatus->pos().x(), (int)(windowHeigth * this->lbStatusPosition.y() / this->originalWindowSize.height()));
  ui->lbStatus->resize(ui->lbStatus->geometry().size().width(), windowHeigth * this->lbStatusSize.height() / this->originalWindowSize.height());
  ui->lbStatus->resize(windowWidth * this->lbStatusSize.width() / this->originalWindowSize.width(), ui->lbStatus->geometry().size().height());

  ui->lbStatusTitle->move((int)(windowWidth * this->lbStatusTitlePosition.x() / this->originalWindowSize.width()), ui->lbStatusTitle->pos().y());
  ui->lbStatusTitle->move(ui->lbStatusTitle->pos().x(), (int)(windowHeigth * this->lbStatusTitlePosition.y() / this->originalWindowSize.height()));
  ui->lbStatusTitle->resize(ui->lbStatusTitle->geometry().size().width(), windowHeigth * this->lbStatusTitleSize.height() / this->originalWindowSize.height());
  ui->lbStatusTitle->resize(windowWidth * this->lbStatusTitleSize.width() / this->originalWindowSize.width(), ui->lbStatusTitle->geometry().size().height());

  ui->lbMessages->move((int)(windowWidth * this->lbMessagesPosition.x() / this->originalWindowSize.width()), ui->lbMessages->pos().y());
  ui->lbMessages->move(ui->lbMessages->pos().x(), (int)(windowHeigth * this->lbMessagesPosition.y() / this->originalWindowSize.height()));
  ui->lbMessages->resize(ui->lbMessages->geometry().size().width(), windowHeigth * this->lbMessagesSize.height() / this->originalWindowSize.height());
  ui->lbMessages->resize(windowWidth * this->lbMessagesSize.width() / this->originalWindowSize.width(), ui->lbMessages->geometry().size().height());

  ui->checkBoxDatabase->move((int)(windowWidth * this->checkBoxDatabasePosition.x() / this->originalWindowSize.width()), ui->checkBoxDatabase->pos().y());
  ui->checkBoxDatabase->move(ui->checkBoxDatabase->pos().x(), (int)(windowHeigth * this->checkBoxDatabasePosition.y() / this->originalWindowSize.height()));
  ui->checkBoxDatabase->resize(ui->checkBoxDatabase->geometry().size().width(), windowHeigth * this->checkBoxDatabaseSize.height() / this->originalWindowSize.height());
  ui->checkBoxDatabase->resize(windowWidth * this->checkBoxDatabaseSize.width() / this->originalWindowSize.width(), ui->checkBoxDatabase->geometry().size().height());

  ui->lvComponents->move((int)(windowWidth * this->lvComponentsPosition.x() / this->originalWindowSize.width()), ui->lvComponents->pos().y());
  ui->lvComponents->move(ui->lvComponents->pos().x(), (int)(windowHeigth * this->lvComponentsPosition.y() / this->originalWindowSize.height()));
  ui->lvComponents->resize(ui->lvComponents->geometry().size().width(), windowHeigth * this->lvComponentsSize.height() / this->originalWindowSize.height());
  ui->lvComponents->resize(windowWidth * this->lvComponentsSize.width() / this->originalWindowSize.width(), ui->lvComponents->geometry().size().height());

  ui->lvConfigurations->move((int)(windowWidth * this->lvConfigurationsPosition.x() / this->originalWindowSize.width()), ui->lvConfigurations->pos().y());
  ui->lvConfigurations->move(ui->lvConfigurations->pos().x(), (int)(windowHeigth * this->lvConfigurationsPosition.y() / this->originalWindowSize.height()));
  ui->lvConfigurations->resize(ui->lvConfigurations->geometry().size().width(), windowHeigth * this->lvConfigurationsSize.height() / this->originalWindowSize.height());
  ui->lvConfigurations->resize(windowWidth * this->lvConfigurationsSize.width() / this->originalWindowSize.width(), ui->lvConfigurations->geometry().size().height());

  ui->lvConfigBOOT->move((int)(windowWidth * this->lvConfigBOOTPosition.x() / this->originalWindowSize.width()), ui->lvConfigBOOT->pos().y());
  ui->lvConfigBOOT->move(ui->lvConfigBOOT->pos().x(), (int)(windowHeigth * this->lvConfigBOOTPosition.y() / this->originalWindowSize.height()));
  ui->lvConfigBOOT->resize(ui->lvConfigBOOT->geometry().size().width(), windowHeigth * this->lvConfigBOOTSize.height() / this->originalWindowSize.height());
  ui->lvConfigBOOT->resize(windowWidth * this->lvConfigBOOTSize.width() / this->originalWindowSize.width(), ui->lvConfigBOOT->geometry().size().height());

  double quadraticMeanConfigurationFontSize = (double)qSqrt((qreal)(windowHeigth * windowHeigth + windowWidth * windowWidth));

  QFont gbDAQInterfaceCommandsFont_("Cantarell", 11);
  int lbFontSizeDAQInterfaceCommands = (int)(this->gbDAQInterfaceCommandsFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
  gbDAQInterfaceCommandsFont_.setPointSize(lbFontSizeDAQInterfaceCommands);
  ui->groupBox_DAQInterfaceCommands->setFont(gbDAQInterfaceCommandsFont_);

  QFont gbDAQInterfaceFont_("Cantarell", 11);
  int lbFontSizeDAQInterface = (int)(this->gbDAQInterfaceFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
  gbDAQInterfaceFont_.setPointSize(lbFontSizeDAQInterface);
  ui->groupBox_DAQInterface->setFont(gbDAQInterfaceFont_);

  QFont bRunFont_("Cantarell", 11);
  int lbFontSizeRun = (int)(this->bRunFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
  bRunFont_.setPointSize(lbFontSizeRun);
  ui->bRun->setFont(bRunFont_);

  QSize bRunIconResize = this->bRunIconSize * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize;
  ui->bRun->setIconSize(bRunIconResize);

  if(this->EnableFontAutoResizing) {
    QFont taDAQInterfaceFont_("Cantarell", 11);
    taDAQInterfaceFont_.setFamily(ui->taDAQInterface->font().family());
    int taFontSizeDAQInterface = (int)(this->taDAQInterfaceFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    taDAQInterfaceFont_.setPointSize(taFontSizeDAQInterface);
    ui->taDAQInterface->setFont(taDAQInterfaceFont_);

    QFont lvComponentsFont_("Cantarell", 11);
    lvComponentsFont_.setFamily(ui->lvComponents->font().family());
    int lvFontSizeComponents = (int)(this->lvComponentsFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    lvComponentsFont_.setPointSize(lvFontSizeComponents);
    ui->lvComponents->setFont(lvComponentsFont_);

    QFont lvConfigurationsFont_("Cantarell", 11);
    lvConfigurationsFont_.setFamily(ui->lvConfigurations->font().family());
    int lvFontSizeConfigurations = (int)(this->lvConfigurationsFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    lvConfigurationsFont_.setPointSize(lvFontSizeConfigurations);
    ui->lvConfigurations->setFont(lvConfigurationsFont_);

    QFont lvBOOTConfigFont_("Cantarell", 11);
    lvBOOTConfigFont_.setFamily(ui->lvConfigBOOT->font().family());
    int lvFontSizeBOOTConfig = (int)(this->lvBOOTConfigFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    lvBOOTConfigFont_.setPointSize(lvFontSizeBOOTConfig);
    ui->lvConfigBOOT->setFont(lvBOOTConfigFont_);
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::openMenuOptionsDialog()
{
  qDebug() << "Starting" << Q_FUNC_INFO;
  MenuOptionsDialog* menuOptionsDialog = new MenuOptionsDialog(this);
  menuOptionsDialog->setWindowTitle("Options");
  menuOptionsDialog->setFromMainWindowFont(ui->taDAQInterface->font());
  menuOptionsDialog->setupFontComboBox();
  menuOptionsDialog->setEnableAutoResizing(this->EnableFontAutoResizing);
  menuOptionsDialog->setupCheckBoxEnableAutoResizing();
  menuOptionsDialog->setEnableShellScripts(this->enableShellScripts);
  int result = menuOptionsDialog->exec();
  if(result == QDialog::Accepted) {
    QFont font(menuOptionsDialog->getFontType(), menuOptionsDialog->getFontSize());
    ui->taDAQInterface->setFont(font);
    ui->lvComponents->setFont(font);
    ui->lvConfigurations->setFont(font);
    ui->lvConfigBOOT->setFont(font);
    ui->taDAQInterface->setText(this->daqInterfaceTextAreaLog);
    this->EnableFontAutoResizing = menuOptionsDialog->getEnableAutoResizing();
    this->enableShellScripts = menuOptionsDialog->getEnableShellScripts();
    commDAQInterface.setShellScripts(this->enableShellScripts);
    this->resizeWindow();
  }
  else {

  }
  delete menuOptionsDialog;
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::openConfigDialog(){
  qDebug() << "Starting" << Q_FUNC_INFO;
  QMessageBox msgBox;
  QString str_status = commDAQInterface.getDAQInterfaceStatus();
  if(status_map_int.value(str_status) == 1){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open configuration"), "", tr("Configuration Files (*.txt)"));
    if(fileName != ""){
      this->retrieveConfigFromFile(fileName);
      qInfo() << "Opened file: " << fileName;
    }
  }else{
    msgBox.setText("Open Config operation is only permitted in 'Stopped' state");
    msgBox.exec();
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::saveConfigDialog(){
  qDebug() << "Starting" << Q_FUNC_INFO;
  QMessageBox msgBox;
  if(this->list_comps_selected.length() == 0){
    msgBox.setText("Selected components list is empty");
    msgBox.exec();
  }else if(this->list_config_selected.length() == 0){
    msgBox.setText("Selected configurations list is empty");
    msgBox.exec();
  }else if(this->list_BOOTConfig_selected.length() == 0){
    msgBox.setText("Selected BOOT list is empty");
    msgBox.exec();
  }else{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save configuration"), ".txt", tr("Configuration Files (*.txt)"));
    if(fileName != "")
    {
      this->saveRunConfig(fileName);
      qInfo() << "Saving configuration in file: " << fileName;
    }
  }
  qDebug() << "Ending" << Q_FUNC_INFO;
}

void MainWindow::MensajeParaBelen()
{
  // podemos dejar esto ;)
  ui->taDAQInterface->document()->setPlainText("Sos la mujer mas hermosa, no puedo dejar de amarte. Sos una bendicion en mi vida. Te quiero muchisimo");
}
