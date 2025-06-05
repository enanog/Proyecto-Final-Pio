#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage fondo(":/Imagenes/logo.png");
    QImage fondoBlanco;
    fondoBlanco.fill(Qt::white);
    fondoBlanco.scaled(QSize(500,500));
    ui->label->setPixmap(QPixmap::fromImage(fondoBlanco));
    ui->label_3->setPixmap(QPixmap::fromImage(fondoBlanco));
    //ui->label->hide();
    //ui->label_3->hide();
    fondo = escalarImagenMin(160,120, fondo);
    ui->label_4->setFixedSize(fondo.size());

    ui->label_4->setPixmap(QPixmap::fromImage(fondo));
    QPixmap bkgnd(":/Imagenes/varioshilos.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    enumerarPuertos();
    connect(&puerto, SIGNAL(readyRead()), this, SLOT(on_datosRecibidos()));

    MainWindow::setWindowTitle("Aplicación Dora");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::resetButtonStyle);
    timer->setInterval(200);  // Ajusta el tiempo en milisegundos según tus necesidades
    timer->setSingleShot(true); // Solo se activa una vez

    tiempoBordado->setInterval(5);
    connect(tiempoBordado, &QTimer::timeout, this, &MainWindow::mostrarBordado);

    ui->pbEnviar->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::resizeEvent(QResizeEvent* evt)
{
    QPixmap bkgnd(":/Imagenes/varioshilos.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette;

    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    QMainWindow::resizeEvent(evt); // call inherited implementation
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setStyleSheet("background-color: rgba(50, 50, 50, 178); border: 2px solid white; border-radius: 10px; color: white;");
    QTimer::singleShot(200, this, &MainWindow::resetButtonStyle);  // Restablecer después de 1000 ms (1 segundo)

    limpiarMatriz();
    QImage fondoBlanco;
    fondoBlanco.fill(Qt::white);
    fondoBlanco.scaled(QSize(500,500));

    ui->label->setPixmap(QPixmap::fromImage(fondoBlanco));
    ui->label_3->setPixmap(QPixmap::fromImage(fondoBlanco));

    ui->label_5->clear();

    tiempoBordado->stop();

    csvData.clear();
    datosDisplay.clear();

    envio = false;
    cantComandosImagen = 0;
    fila = 0;
    porcentajeBordado = 0;

    envioDisplay = false;
    filaDisplay = 0;
    porcentajeDisplay = 0;

    indiceLinea = 0;

    mostrarEnviar = false;

    imagenBordado.fill(Qt::white);
    image.fill(Qt::white);
    imagenContorno.fill(Qt::white);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_3->setStyleSheet("background-color: rgba(50, 50, 50, 178); border: 2px solid white; border-radius: 10px; color: white;");
    QTimer::singleShot(200, this, &MainWindow::resetButtonStyle);  // Restablecer después de 1000 ms (1 segundo)
    Instrucciones ventana(this);
    ventana.setWindowTitle("Instrucciones");
    ventana.exec();
}



void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_2->setStyleSheet("background-color: rgba(50, 50, 50, 178); border: 2px solid white; border-radius: 10px; color: white;");
    QTimer::singleShot(200, this, &MainWindow::resetButtonStyle);  // Restablecer después de 1000 ms (1 segundo)
    QString folderPath = QFileDialog::getExistingDirectory(this, "Selecciona una carpeta", QDir::homePath());

    ui->label->show();
    ui->label_3->show();

    if (!folderPath.isEmpty()) {
        QDir folderDir(folderPath);
        QStringList filters;
        filters << "*.png" << "*.csv";
        QStringList fileNames = folderDir.entryList(filters, QDir::Files);

        QString pngFileName;
        QString csvFileName;

        foreach (const QString &fileName, fileNames) {
            if (fileName.endsWith(".png") || fileName.endsWith(".PNG") || fileName.endsWith(".jpg") || fileName.endsWith(".JPG")) {
                pngFileName = folderDir.filePath(fileName);
            } else if (fileName.endsWith(".csv")) {
                csvFileName = folderDir.filePath(fileName);
            }
        }

        procesarImagen(pngFileName, true);

        procesarCsv(csvFileName);
    }
}

void MainWindow::procesarImagen(QString fn, bool mostrarImagen)
{
    // Limpio todas las matrices
    limpiarMatriz();
    setearColores();

    QImage imagenOriginal(fn);

    // Escalar la imagen a las nuevas dimensiones
    image = escalarImagenMin(maxAncho, maxAlto, imagenOriginal);
    image = escalarImagenMax(minAncho, minAlto, image);

    // Configuro las imágenes
    imagenContorno = QImage(image.size(), QImage::Format_RGB32);
    imagenContorno.fill(Qt::white);


    // Separo la imagen por colores
    recorrerImagen(image);

    if(!mostrarImagen){
        for(int i = 0; i < CANT_COLORES; i++){
            for(int k = 0; k < matriz[i].pixel.size(); k++){
                matriz[i].pixel[k].x += (320 - image.width()) / 2;
                matriz[i].pixel[k].y += (240 - image.height()) / 2;
            }
        }
    }

    //Genero la nueva Imagen
    for(int i = 0; i < CANT_COLORES; i++){
        if(!matriz[i].pixel.isEmpty())
            pintaMatrizEnImagen(imagenContorno, matriz[i]);
    }

    //Detecto los puntos de inflexion para enviarselo al display
    if(!mostrarImagen){
        for(int i = 0; i < CANT_COLORES; i++){
            if(!matriz[i].pixel.isEmpty())
                matriz[i] = detectarPuntosDeInflexion(matriz[i]);
        }
    }

    //muestro la imagen en la compu
    if(mostrarImagen){
        ui->label->setFixedSize(imagenContorno.size() + QSize(15,15));

        // Muestro la imagen en pantalla
        ui->label->setPixmap(QPixmap::fromImage(imagenContorno));
        ui->label->show();
    }


    if(!mostrarImagen){

        //Seteo el color565 en el valor equivalente de RGB 888 a RGB 565
        for(int i = 0; i < CANT_COLORES; i++){
            if(!matriz[i].pixel.isEmpty())
                matriz[i].color565 = (((coloresPredefinidos[i].red() >> 3) & 0x1F) << 11) | (((coloresPredefinidos[i].green() >> 2) & 0x3F) << 5) | ((coloresPredefinidos[i].blue() >> 3) & 0x1F);
        }

        //Preparo las tramas para enviar al display
        for(int i = 0; i < CANT_COLORES; i++){
            if(matriz[i].pixel.isEmpty())
                continue;

            if(i == 3)
                continue;

            QString text = "$C" + QString("%1").arg(matriz[i].color565, 4, 16, QChar('0')).toUpper() + "#";


            datosDisplay.append(text);
            text.clear();

            for(int k = 0; k < matriz[i].cant; k+=2){
                text = "$D" + QString("%1").arg((int)matriz[i].pixel[k].x, 3, 16, QChar('0')).toUpper() + QString("%1").arg((int)matriz[i].pixel[k].y, 2, 16, QChar('0')).toUpper();
                text += QString("%1").arg((int)matriz[i].pixel[k+1].x, 3, 16, QChar('0')).toUpper() + QString("%1").arg((int)matriz[i].pixel[k+1].y, 2, 16, QChar('0')).toUpper() + "#";
                datosDisplay.append(text);
                text.clear();
            }
        }

    }
    datosDisplay.append("$E#");

}

void MainWindow::procesarCsv(QString filePath)
{
    double extendRight = 0, extendBottom = 0;

    QFile file(filePath);

    QTextStream in(&file);

    csvData.clear();

    if (filePath.isEmpty()) {
        qDebug() << "No se seleccionó ningún archivo.";
        return;
    }

    ui->pbEnviar->setEnabled(true);
    mostrarEnviar = true;

    // Verificar si se pudo abrir el archivo
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo.";
        return;
    }

    while (!in.atEnd()) {
        QString line = in.readLine();

        // Eliminar todas las comillas dobles (") de la línea
        line.remove(QChar('"'));

        QStringList values;
        QString currentValue;

        bool insideQuotes = false;
        for (int i = 0; i < line.length(); ++i) {
            QChar currentChar = line[i];

            if (currentChar == ',' && !insideQuotes) {
                values.append(currentValue.trimmed());
                currentValue.clear();
            } else if (currentChar == '"') {
                insideQuotes = !insideQuotes;
            } else {
                currentValue += currentChar;
            }
        }

        // Agregar el último valor
        values.append(currentValue.trimmed());

        // Agregar la línea completa como una lista a la lista principal
        csvData.append(values);
    }

    // Cerrar el archivo
    file.close();

    int grupo = 0;
    int nColor = 0;
    QList<QString> ordenColores;
    setearColoresCsv();
    bool aux = false;
    QList<QString> nuevaFila;

    for (int fila = 0; fila < csvData.size(); fila++) {

        const QList<QString>& row = csvData[fila];

        if (csvData[fila][0] == "#") {
            grupo++;
            continue;
        }
        if (fila == 6)
            extendRight = row[2].toDouble();

        if (fila == 7)
            extendBottom = row[2].toDouble();

        if (grupo == 3 && row.size() > 1) {
            QString codigoHexadecimal = row[2];

            // Buscar el código hexadecimal en el mapa y obtener la sigla de color
            QString siglaColor = mapaColores.value(codigoHexadecimal, "");

            // Agregar la sigla de color a ordenColores
            ordenColores.append(siglaColor);
        }

        if (row[0] == "*") {
            if(aux == false){
                nuevaFila << "*" << "0" << "COLOR_SWITCH" << csvData[fila+1][3] << csvData[fila+1][4];
                csvData.insert(fila,nuevaFila);
                nuevaFila.clear();

                nuevaFila << "*" << "0" << "T" << csvData[8][2] << csvData[9][2];
                csvData.insert(fila,nuevaFila);
                nuevaFila.clear();

                nuevaFila << "*" << "0" << "W" << QString::number(csvData[10][2].toInt() + csvData[11][2].toInt())<< "0";
                csvData.insert(fila,nuevaFila);
                nuevaFila.clear();

                nuevaFila << "*" << "0" << "P";
                QString colores;
                for(int i = 0; i < ordenColores.size(); i++){
                    colores.append(ordenColores[i]);
                }
                nuevaFila << colores << "0";
                csvData.insert(fila,nuevaFila);
                aux = true;
            }
            if(row[2] == "W"){
                csvData[fila][3] = QString("%1").arg(((int)qRound(row[3].toDouble())), 4, 16, QChar('0')).toUpper();
            }
            if((row[2].startsWith("J")) || (row[2].startsWith("S"))){
                csvData[fila][3] = QString("%1").arg(((int)qRound(row[3].toDouble() + extendRight)), 3, 16, QChar('0')).toUpper();
                csvData[fila][4] = QString("%1").arg(((int)qRound(row[4].toDouble() + extendBottom)), 3, 16, QChar('0')).toUpper();
            }
            if(row[2] == "T"){
                csvData[fila][3] = QString("%1").arg(((int)qRound(row[3].toDouble())), 3, 16, QChar('0')).toUpper();
                csvData[fila][4] = QString("%1").arg(((int)qRound(row[4].toDouble())), 3, 16, QChar('0')).toUpper();
            }
            if(row[2][0] == 'C'){
                csvData[fila][2] = "H";
                csvData[fila][3] = ordenColores[nColor];
                csvData[fila].removeAt(4);
                nColor++;
            }

            if(row[2][0] == 'S'){
                csvData[fila][2] = "S";
            }
            if(row[2][0] == 'J'){
                csvData[fila][2] = "J";
            }
            if(row[2][0] == 'E'){
                csvData[fila][2] = "E";
            }
        }
    }

    while((csvData[0][0] == "#") || (csvData[0][0] == ">") || (csvData[0][0] == "@") || (csvData[0][0] == "$") || (csvData[0][0] == "")){
        csvData.removeAt(0);
    }

    for (int fila = 0; fila < csvData.size(); fila++){
        csvData[fila].removeAt(1);
        csvData[fila][0] = "$";
        csvData[fila].append("#");
        if((csvData[fila][1] == "W") || (csvData[fila][1] == "P")){
            csvData[fila].removeAt(3);
        }
        if(csvData[fila][1] == "E"){
            csvData[fila].removeAt(2);
            csvData[fila].removeAt(2);
        }
    }

    for(int i = 0; i < csvData.size(); i++){
        qDebug() << csvData[i];
    }

    imagenBordado = imagenContorno.copy(0, 0, imagenContorno.width(), imagenContorno.height());
    imagenBordado.fill(Qt::white);
    ui->label_3->setFixedSize(imagenContorno.size() + QSize(15, 15));

    int tiempoMinutos = ((double)(csvData.size()) / 3.587) / 60;
    int tiempoSegundos = (int)((double)(csvData.size()) / 3.587) % 60;
    ui->label_5->setText("Tiempo Aproximado: \n"  + QString::number(tiempoMinutos) + " min " + QString::number(tiempoSegundos) + " seg ");

    indiceLinea = 0;
    mostrarBordado();
    tiempoBordado->start();
}

void MainWindow::mostrarBordado(){

    static int maxX = 0;
    static int maxY = 0;

    static double relacionMmToPixelX = 0;
    static double relacionMmToPixelY = 0;

    // Variables para el seguimiento de las coordenadas de inicio
    int startX = 0;
    int startY = 0;

    int endX = 0;
    int endY = 0;

    static QString colorActual;

    QPainter painter(&imagenBordado);
    painter.setRenderHint(QPainter::Antialiasing);

    bool ok = true;

    if(csvData[indiceLinea][1] == "E"){
        imagenBordado.fill(Qt::white);
        indiceLinea = 0;
    }

    if((csvData[indiceLinea][1] == "W") || (csvData[indiceLinea][1] == "P")){
        indiceLinea++;
    }


    if(csvData[indiceLinea][1] == "T"){
        maxX = csvData[indiceLinea][2].toInt(&ok, 16);
        maxY = csvData[indiceLinea][3].toInt(&ok, 16);

        relacionMmToPixelX = (double)maxX/(double)imagenBordado.width();
        relacionMmToPixelY = (double)maxY/(double)imagenBordado.height();

        indiceLinea++;
    }

    if(csvData[indiceLinea][1] == "H"){
        for (const QString &hexadecimal : mapaColores.keys()) {
            if (mapaColores[hexadecimal] == csvData[indiceLinea][2]) {
                colorActual = hexadecimal;
            }
        }
        indiceLinea++;
    }

    painter.setPen(QColor(colorActual));

    if(csvData[indiceLinea][1] == "J" || csvData[indiceLinea][1] == "S"){
        if((csvData[indiceLinea+1][1] != "H") && (csvData[indiceLinea+1][1] != "W") && (csvData[indiceLinea+1][1] != "E")){
            startX = (double)csvData[indiceLinea][2].toInt(&ok, 16) / (double)relacionMmToPixelX;
            startY = (double)csvData[indiceLinea][3].toInt(&ok, 16) / (double)relacionMmToPixelY;
            endX = (double)csvData[indiceLinea+1][2].toInt(&ok, 16) / (double)relacionMmToPixelX;
            endY = (double)csvData[indiceLinea+1][3].toInt(&ok, 16) / (double)relacionMmToPixelY;
            }
        indiceLinea++;
    }

    painter.drawLine(startX, startY, endX, endY);

    // Muestro la imagen en pantalla
    ui->label_3->setPixmap(QPixmap::fromImage(imagenBordado));
    ui->label_3->show();

}

void MainWindow::limpiarMatriz(){

    for(char i = 0; i < CANT_COLORES; i++){
        matriz[i].pixel.clear();
        matriz[i].cant = 0;
        matriz[i].color = i;
        matriz[i].color565 = 0;
    }
}

void MainWindow::limpiarMatriz(matrizInfo_t &matriz){

    matriz.pixel.clear();
    matriz.cant = 0;
}

void MainWindow::setearColoresCsv(){

    mapaColores["#ffff00"] = "AM";    // Amarillo
    mapaColores["#000000"] = "NG";    // Negro
    mapaColores["#ffffff"] = "BL";    // Blanco
    mapaColores["#0000ff"] = "AZ";    // Azul
    mapaColores["#00007b"] = "AO";    // Azul Oscuro
    mapaColores["#7b0000"] = "BO";    // Bordo
    mapaColores["#00ffff"] = "CN";    // Cian
    mapaColores["#007d7b"] = "CO";    // Cian Oscuro
    mapaColores["#c6c3c6"] = "GC";    // Gris Claro
    mapaColores["#7b7b7b"] = "GO";    // Gris Oscuro
    mapaColores["#ff00ff"] = "MG";    // Magenta
    mapaColores["#8b4513"] = "MN";    // Marrón
    mapaColores["#ffa500"] = "NJ";    // Naranja
    mapaColores["#808000"] = "OL";    // Oliva
    mapaColores["#7b0080"] = "PR";    // Púrpura
    mapaColores["#ff0000"] = "RJ";    // Rojo
    mapaColores["#ff82c6"] = "RS";    // Rosa
    mapaColores["#00ff00"] = "VD";    // Verde
    mapaColores["#adff29"] = "VA";    // Verde Amarillo
    mapaColores["#007d00"] = "VO";    // Verde Oscuro
}

void MainWindow::setearColores(){
    coloresPredefinidos << QColor(255, 255, 0);      // Amarillo
    coloresPredefinidos << QColor(0, 0, 255);        // Azul
    coloresPredefinidos << QColor(0, 0, 123);        // Azul Oscuro
    coloresPredefinidos << QColor(255, 255, 255);    // Blanco
    coloresPredefinidos << QColor(123, 0, 0);        // Bordo
    coloresPredefinidos << QColor(0, 255, 255);      // Cian
    coloresPredefinidos << QColor(0, 125, 123);      // Cian Oscuro
    coloresPredefinidos << QColor(198, 195, 198);    // Gris Claro
    coloresPredefinidos << QColor(123, 123, 123);    // Gris Oscuro
    coloresPredefinidos << QColor(255, 0, 255);      // Magenta
    coloresPredefinidos << QColor(139, 69, 19);      // Marrón
    coloresPredefinidos << QColor(0, 0, 0);          // Negro
    coloresPredefinidos << QColor(255, 165, 0);      // Naranja
    coloresPredefinidos << QColor(128, 128, 0);      // Oliva
    coloresPredefinidos << QColor(123, 0, 128);      // Púrpura
    coloresPredefinidos << QColor(255, 0, 0);        // Rojo
    coloresPredefinidos << QColor(255, 130, 198);    // Rosa
    coloresPredefinidos << QColor(0, 255, 0);        // Verde
    coloresPredefinidos << QColor(173, 255, 41);     // Verde Amarillo
    coloresPredefinidos << QColor(0, 125, 0);        // Verde Oscuro
    coloresPredefinidos << QColor(255, 192, 203);    // Rosado Claro
    coloresPredefinidos << QColor(205, 92, 92);      // Marrón Indian
    coloresPredefinidos << QColor(188, 143, 143);    // Rosy Brown
    coloresPredefinidos << QColor(255, 222, 173);    // Navajo White
    coloresPredefinidos << QColor(238, 130, 238);    // Violeta
    coloresPredefinidos << QColor(255, 20, 147);     // Rosa Profundo
    coloresPredefinidos << QColor(255, 192, 203);    // Rosy Brown
    coloresPredefinidos << QColor(199, 21, 133);     // Púrpura Medio
    coloresPredefinidos << QColor(255, 69, 0);       // Rojo Tomate
    coloresPredefinidos << QColor(0, 250, 154);      // Verde Medio Marino
    coloresPredefinidos << QColor(210, 105, 30);     // Chocolate
    coloresPredefinidos << QColor(255, 218, 185);    // Salmón Claro
    coloresPredefinidos << QColor(255, 99, 71);      // Rojo Tomate
    coloresPredefinidos << QColor(154, 205, 50);     // Verde Amarillo Claro
    coloresPredefinidos << QColor(139, 0, 139);      // Violeta Oscuro
    coloresPredefinidos << QColor(255, 182, 193);    // Rosado Claro
    coloresPredefinidos << QColor(255, 105, 180);    // Rosa Intenso
    coloresPredefinidos << QColor(255, 250, 250);    // Rosa Nieve
    coloresPredefinidos << QColor(240, 230, 140);    // Amarillo Khaki
    coloresPredefinidos << QColor(210, 180, 140);    // Marrón Arena
    coloresPredefinidos << QColor(255, 0, 127);      // Rosa Fuerte
    coloresPredefinidos << QColor(75, 0, 130);       // Índigo
    coloresPredefinidos << QColor(107, 142, 35);     // Verde Oliva
    coloresPredefinidos << QColor(255, 69, 0);       // Rojo Rojo
    coloresPredefinidos << QColor(218, 112, 214);    // Orquídea
    coloresPredefinidos << QColor(128, 0, 0);        // Rojo Granate
    coloresPredefinidos << QColor(0, 128, 0);        // Verde Bosque
    coloresPredefinidos << QColor(0, 255, 127);      // Verde Menta
    coloresPredefinidos << QColor(255, 0, 255);      // Rosa Púrpura
    coloresPredefinidos << QColor(176, 224, 230);    // Azul Polvo
    coloresPredefinidos << QColor(0, 128, 128);      // Verde Azul
    coloresPredefinidos << QColor(255, 228, 225);    // Rosa Melocotón
    coloresPredefinidos << QColor(46, 139, 87);      // Verde Mar
    coloresPredefinidos << QColor(255, 215, 0);      // Amarillo Oro
    coloresPredefinidos << QColor(160, 82, 45);      // Marrón Siena
    coloresPredefinidos << QColor(255, 239, 139);    // Amarillo Seda
    coloresPredefinidos << QColor(255, 105, 180);    // Rosa Intenso
    coloresPredefinidos << QColor(255, 140, 0);      // Naranja Intenso
    coloresPredefinidos << QColor(173, 255, 47);     // Verde Lima
    coloresPredefinidos << QColor(219, 112, 147);    // Rosa Rosa
    coloresPredefinidos << QColor(32, 178, 170);     // Verde Mar Oscuro
    coloresPredefinidos << QColor(255, 127, 80);     // Coral
    coloresPredefinidos << QColor(139, 0, 0);        // Rojo Oscuro
    coloresPredefinidos << QColor(255, 20, 147);     // Rosa Profundo
    coloresPredefinidos << QColor(0, 0, 128);        // Azul Marino
    coloresPredefinidos << QColor(165, 42, 42);      // Marrón Rojizo
    coloresPredefinidos << QColor(0, 255, 0);        // Verde Brillante
    coloresPredefinidos << QColor(255, 69, 0);       // Rojo Rojo
    coloresPredefinidos << QColor(218, 112, 214);    // Orquídea
    coloresPredefinidos << QColor(128, 0, 0);        // Rojo Granate
    coloresPredefinidos << QColor(0, 128, 0);        // Verde Bosque
    coloresPredefinidos << QColor(0, 255, 127);      // Verde Menta
}

void MainWindow::recorrerImagen(QImage imagen){
    QRgb pixelColor;
    QColor colorCercano;
    coordenadas_t nuevoElemento;

    for (unsigned char y = 0; y < imagen.height(); y++) {
        for (unsigned short x = 0; x < imagen.width(); x++) {
            pixelColor = imagen.pixel(x, y);
            nuevoElemento.x = x;
            nuevoElemento.y = y;
            if(qAlpha(pixelColor) == 0){
                continue;
            }

            colorCercano = encontrarColorMasCercano(pixelColor, coloresPredefinidos);

            for(int i = 0; i < CANT_COLORES; i++){
                if(colorCercano == coloresPredefinidos[i]){
                    matriz[i].pixel.push_back(nuevoElemento);
                    matriz[i].cant++;
                    break;
                }
            }
        }
    }
}

void MainWindow::pintaMatrizEnImagen(QImage &imagen, matrizInfo_t &matriz)
{
    for (int i = 0; i < matriz.cant; i++) {
        coordenadas_t image;
        image.x = matriz.pixel[i].x;
        image.y = matriz.pixel[i].y;
        if (image.x >= 0 && image.x < imagen.width() && image.y >= 0 && image.y < imagen.height())
            imagen.setPixel(matriz.pixel[i].x, matriz.pixel[i].y, coloresPredefinidos[matriz.color].rgb());
    }
}

QImage MainWindow::escalarImagenMin(int maxAncho, int maxAlto, QImage& imagen){

    // Calcular las nuevas dimensiones manteniendo la proporción
    double nuevoAncho, nuevoAlto;

    if (imagen.width() > maxAncho || imagen.height() > maxAlto) {
        double proporcionAncho = (maxAncho) / (double)imagen.width();
        double proporcionAlto = (maxAlto) / (double)imagen.height();

        double proporcionEscalado = qMin(proporcionAncho, proporcionAlto);

        nuevoAncho = (int)(imagen.width() * proporcionEscalado);
        nuevoAlto = (int)(imagen.height() * proporcionEscalado);
    } else {
        nuevoAncho = imagen.width();
        nuevoAlto = imagen.height();
    }

    return imagen.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio);
}

QImage MainWindow::escalarImagenMax(int minAncho, int minAlto, QImage& imagen){

    // Calcular las nuevas dimensiones manteniendo la proporción
    double nuevaAncho, nuevaAlto;

    if (imagen.width() < minAncho || imagen.height() < minAlto) {
        double proporcionAncho = (minAncho) / (double)imagen.width();
        double proporcionAlto = (minAlto) / (double)imagen.height();

        double proporcionEscalado = qMin(proporcionAncho, proporcionAlto);

        nuevaAncho = (int)(imagen.width() * proporcionEscalado);
        nuevaAlto = (int)(imagen.height() * proporcionEscalado);
    } else {
        nuevaAncho = imagen.width();
        nuevaAlto = imagen.height();
    }

    return imagen.scaled(nuevaAncho, nuevaAlto, Qt::KeepAspectRatio);
}

QColor MainWindow::encontrarColorMasCercano(const QColor& color, const QList<QColor>& colores) {
    double distanciaMinima = std::numeric_limits<double>::max();
    QColor colorMasCercano;

    foreach (const QColor& colorActual, colores) {
        double distancia = calcularDistancia(color, colorActual);
        if (distancia < distanciaMinima) {
            distanciaMinima = distancia;
            colorMasCercano = colorActual;
        }
    }

    return colorMasCercano;
}

double MainWindow::calcularDistancia(const QColor& color1, const QColor& color2) {
    int r1 = color1.red();
    int g1 = color1.green();
    int b1 = color1.blue();

    int r2 = color2.red();
    int g2 = color2.green();
    int b2 = color2.blue();

    return sqrt(pow(r2 - r1, 2) + pow(g2 - g1, 2) + pow(b2 - b1, 2));
}

matrizInfo_t MainWindow::detectarPuntosDeInflexion(matrizInfo_t &matriz){
    matrizInfo_t resultado;

    for (const coordenadas_t& coordenada : matriz.pixel) {
        bool cumpleCondicion = true;
        bool pixelAislado = true;
        bool pixelColor = false;
        char pixelAdyacente = 0;
        for (const coordenadas_t& otraCoordenada : matriz.pixel) {
            if (&coordenada != &otraCoordenada) { // Evita comparar una coordenada consigo misma
                if(((coordenada.x - 1 == otraCoordenada.x) && (coordenada.y == otraCoordenada.y)) ||
                    ((coordenada.x + 1 == otraCoordenada.x) && (coordenada.y == otraCoordenada.y))){
                    pixelAdyacente++;
                    pixelAislado = false;
                }
            }
            else
                pixelColor = true;

            if(pixelAdyacente == 2){
                cumpleCondicion = false;
                break;
            }
        }

        if (cumpleCondicion) {
            resultado.pixel.append(coordenada);
            if(pixelAislado && pixelColor)
                resultado.pixel.append(coordenada);
        }
    }

    resultado.cant = resultado.pixel.size();
    resultado.color = matriz.color;

    return resultado;
}

void MainWindow::enviarFigura()
{
    QByteArray datos;
    char aux = 0;
    QString text;
    envioDisplay = true;
    filaDisplay = 0;
    envioVacio = false;

    while(filaDisplay < 100 && envioDisplay && filaDisplay < datosDisplay.size()){
        text = datosDisplay[filaDisplay];
        datos.append(text.toUtf8());
        for(int k = 0; k < datos.size(); k++){
            aux = datos.at(k);
            puerto.flush();
            puerto.write(&aux, 1);
        }
        qDebug() << datosDisplay[filaDisplay];
        datos.clear();
        text.clear();
        filaDisplay++;
    }

}

void MainWindow::enumerarPuertos()
{
    ui->cbPuertos->clear(); // Limpio antes de actualizar la lista para que no se haga infinita
    QList<QSerialPortInfo> puertos = QSerialPortInfo::availablePorts();
    for(int i = 0; i < puertos.count(); i++)
    {
        ui->cbPuertos->addItem(puertos.at(i).portName());
    }
}

void MainWindow::on_pbActualizar_clicked()
{
    ui->pbActualizar->setStyleSheet("background-color: rgba(50, 50, 50, 178); border: 2px solid white; color: white;");
    QTimer::singleShot(200, this, &MainWindow::resetButtonStyle);  // Restablecer después de 1000 ms (1 segundo)
    enumerarPuertos();
}
void MainWindow::resetButtonStyle()
{
    ui->pbActualizar->setStyleSheet("background-color: rgba(0, 0, 0, 178); border: 2px solid white; color: white;");
    ui->pbConectar->setStyleSheet("background-color: rgba(0, 0, 0, 178); border: 2px solid white; color: white;");
    ui->pbEnviar->setStyleSheet("background-color: rgba(0, 0, 0, 178); border: 2px solid white; color: white;");
    ui->pushButton_3->setStyleSheet("background-color: rgba(0, 0, 0, 178); border: 2px solid white; border-radius: 10px; color: white;");
    ui->pushButton_2->setStyleSheet("background-color: rgba(0, 0, 0, 178); border: 2px solid white; border-radius: 10px; color: white;");
    ui->pushButton->setStyleSheet("background-color: rgba(0, 0, 0, 178); border: 2px solid white; border-radius: 10px; color: white;");
}

void MainWindow::on_pbConectar_clicked()
{
    ui->pbConectar->setStyleSheet("background-color: rgba(50, 50, 50, 178); border: 2px solid white; color: white;");
    QTimer::singleShot(200, this, &MainWindow::resetButtonStyle);  // Restablecer después de 1000 ms (1 segundo)
    if(!puerto.isOpen())
    {
        QString portName = ui->cbPuertos->currentText();
        puerto.setPortName(portName);
        puerto.setBaudRate(115200);
        puerto.setDataBits(QSerialPort::Data8);
        puerto.setParity(QSerialPort::NoParity);
        puerto.setStopBits(QSerialPort::OneStop);
        puerto.setFlowControl(QSerialPort::NoFlowControl);
        if(puerto.open(QIODevice::ReadWrite) == true)
        {
            ui->pbConectar->setText("Desconectar");
            ui->statusbar->showMessage(portName + " conectado OK", 5000); // Muestro msg en la status bar por un tiempo

        } else {
            QMessageBox::critical(this, "Error", "No se pudo abrir el puerto " + portName);
        }
        limpiarMatriz();
        ui->pushButton_2->setEnabled(true);

    } else {
        ui->pushButton_2->setEnabled(false);
        puerto.close();
        ui->pbConectar->setText("Conectar");
        ui->statusbar->showMessage(puerto.portName() + " desconectado OK", 5000);
    }
}

void MainWindow::on_pbEnviar_clicked()
{

    tiempoBordado->stop();

    ui->pbEnviar->setStyleSheet("background-color: rgba(50, 50, 50, 178); border: 2px solid white; color: white;");
    QTimer::singleShot(200, this, &MainWindow::resetButtonStyle);  // Restablecer después de 1000 ms (1 segundo)

    mostrarBordadoCompleto();

    QByteArray datos;
    char aux = 0;
    QString text;
    envio = true;
    fila = 0;
    envioVacio = false;

    mostrarEnviar = false;
    ui->pbEnviar->setEnabled(false);

    while(fila < 101 && envio){
        for(int j = 0; j < csvData[fila].size(); j++){
            text += csvData[fila][j];
        }

        datos.append(text.toUtf8());
        for(int k = 0; k < datos.size(); k++){
            aux = datos.at(k);
            puerto.flush();
            puerto.write(&aux, 1);
        }
        qDebug() << csvData[fila];
        datos.clear();
        text.clear();
        fila++;
    }

    qDebug() << "primeros 101";


}

void MainWindow::mostrarBordadoCompleto(){

    indiceLinea = 0;
    int maxX = 0;
    int maxY = 0;

    double relacionMmToPixelX = 0;
    double relacionMmToPixelY = 0;

    // Variables para el seguimiento de las coordenadas de inicio
    int startX = 0;
    int startY = 0;

    int endX = 0;
    int endY = 0;

    QString colorActual;

    QPainter painter(&imagenBordado);
    painter.setRenderHint(QPainter::Antialiasing);

    bool ok = true;

    imagenBordado.fill(Qt::white);

    while(indiceLinea < csvData.size()){
        if(csvData[indiceLinea][1] == "E"){
            break;
        }

        if((csvData[indiceLinea][1] == "W") || (csvData[indiceLinea][1] == "P")){
            indiceLinea++;
        }

        if(csvData[indiceLinea][1] == "T"){
            maxX = csvData[indiceLinea][2].toInt(&ok, 16);
            maxY = csvData[indiceLinea][3].toInt(&ok, 16);

            relacionMmToPixelX = (double)maxX/(double)imagenBordado.width();
            relacionMmToPixelY = (double)maxY/(double)imagenBordado.height();

            indiceLinea++;
        }

        if(csvData[indiceLinea][1] == "H"){
            for (const QString &hexadecimal : mapaColores.keys()) {
                if (mapaColores[hexadecimal] == csvData[indiceLinea][2]) {
                    colorActual = hexadecimal;
                }
            }
            indiceLinea++;
        }

        painter.setPen(QColor(colorActual));

        if(csvData[indiceLinea][1] == "J" || csvData[indiceLinea][1] == "S"){
            if((csvData[indiceLinea+1][1] != "H") && (csvData[indiceLinea+1][1] != "W") && (csvData[indiceLinea+1][1] != "E")){
                startX = csvData[indiceLinea][2].toInt(&ok, 16) / relacionMmToPixelX;
                startY = csvData[indiceLinea][3].toInt(&ok, 16) / relacionMmToPixelY;
                endX = csvData[indiceLinea+1][2].toInt(&ok, 16) / relacionMmToPixelX;
                endY = csvData[indiceLinea+1][3].toInt(&ok, 16) / relacionMmToPixelY;
            }
            indiceLinea++;
        }

        painter.drawLine(startX, startY, endX, endY);
    }
    // Muestro la imagen en pantalla
    ui->label_3->setPixmap(QPixmap::fromImage(imagenBordado));
    ui->label_3->show();

}

void MainWindow::on_datosRecibidos()
{
    QByteArray datosRx;
    int cant = puerto.bytesAvailable();
    datosRx.resize(cant);
    //datos = puerto.readAll(); // Retorna todo a un qbytearray
    puerto.read(datosRx.data(), cant);
    // Procesamiento de los datos recibidos
    QString textoRX(datosRx);


    QByteArray datosTx;
    datosTx.clear();
    char aux = 0;
    QString text;
    text.clear();
    int cantFilas = 0;

    static int cantComandos = 0;

    qDebug() << textoRX;

    if(envioVacio){
        text = " ";
        datosTx.append(text.toUtf8());
        envioVacio = true;

        for(int k = 0; k < datosTx.size(); k++){
            aux = datosTx.at(k);
            puerto.flush();
            puerto.write(&aux, 1);
        }
        text.clear();
        datosTx.clear();
    }

    if(envioDisplay){
        if(textoRX == "$P#"){
            qDebug() << textoRX;
            textoRX.clear();
            datosRx.clear();
            while((cantComandos < 50) && (filaDisplay < datosDisplay.size())){

                text = datosDisplay[filaDisplay];
                datosTx.append(text.toUtf8());
                for(int k = 0; k < datosTx.size(); k++){
                    aux = datosTx.at(k);
                    puerto.flush();
                    puerto.write(&aux, 1);
                }
                qDebug() << datosDisplay[filaDisplay];
                datosTx.clear();
                text.clear();
                cantComandos++;
                filaDisplay++;
            }

            if(cantComandos == 50)
                cantComandos = 0;
        }
    }

    if(textoRX == "$O#"){
        mostrarEnviar = !mostrarEnviar;

        if(!mostrarEnviar){
            text = " ";
            datosTx.append(text.toUtf8());

            for(int k = 0; k < datosTx.size(); k++){
                aux = datosTx.at(k);
                puerto.flush();
                puerto.write(&aux, 1);
            }
            text.clear();
            datosTx.clear();
        }

        if(mostrarEnviar)
            ui->pbEnviar->setEnabled(true);

        else
            ui->pbEnviar->setEnabled(false);

    }

    if(textoRX == "$Y#"){
        mostrarEnviar = true;
        ui->pbEnviar->setEnabled(true);

        text = " ";
        datosTx.append(text.toUtf8());

        for(int k = 0; k < datosTx.size(); k++){
            aux = datosTx.at(k);
            puerto.flush();
            puerto.write(&aux, 1);
        }
        text.clear();
        datosTx.clear();
    }

    if(textoRX =="$I#"){
        procesarImagen(":/Imagenes/logo.png", false);
        enviarFigura();
    }

    if(envio){
        if(textoRX == "$K#"){
            textoRX.clear();
            datosRx.clear();
            qDebug() << "Nuevos datos...";
            while((cantFilas < 50) && (fila < csvData.size())){
                for(int j = 0; j < csvData[fila].size(); j++){
                    text += csvData[fila][j];
                }

                datosTx.append(text.toUtf8());
                for(int k = 0; k < datosTx.size(); k++){
                    aux = datosTx.at(k);
                    puerto.flush();
                    puerto.write(&aux, 1);
                }

                qDebug() << csvData[fila];
                datosTx.clear();
                text.clear();
                cantFilas++;
                fila++;
                //ui->progressBar->setValue((int)((fila / csvData.size()) * 100));
            }
            cantFilas = 0;
        }

    }


    if((fila >= csvData.size()) && !csvData.isEmpty() && envio){

        if(cantFilas >= 50){

            envio = false;
            text = " ";
            datosTx.append(text.toUtf8());

            for(int k = 0; k < datosTx.size(); k++){
                aux = datosTx.at(k);
                puerto.flush();
                puerto.write(&aux, 1);
            }
            text.clear();
            datosTx.clear();
        }
        else{
            while(cantFilas < 50){

                text = "$E#";
                datosTx.append(text.toUtf8());

                for(int k = 0; k < datosTx.size(); k++){
                    aux = datosTx.at(k);
                    puerto.flush();
                    puerto.write(&aux, 1);
                }
                datosTx.clear();
                text.clear();
                qDebug() << text;
                cantFilas++;
            }

        }
    }


    if((filaDisplay >= datosDisplay.size()) && !datosDisplay.isEmpty() && envioDisplay){

        if(cantComandos >= 50){

            envioDisplay = false;
            text = " ";
            datosTx.append(text.toUtf8());

            for(int k = 0; k < datosTx.size(); k++){
                aux = datosTx.at(k);
                puerto.flush();
                puerto.write(&aux, 1);
            }
            text.clear();
            datosTx.clear();
            datosDisplay.clear();
        }
        else{
            while(cantComandos < 50){

                datosTx.clear();
                text = "$E#";
                datosTx.append(text.toUtf8());

                for(int k = 0; k < datosTx.size(); k++){
                    aux = datosTx.at(k);
                    puerto.flush();
                    puerto.write(&aux, 1);
                }
                datosTx.clear();
                qDebug() << text;
                text.clear();
                cantComandos++;
            }
            ui->pushButton_2->setEnabled(true);
        }
    }

}
