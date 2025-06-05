#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QVector>
#include <QFileDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <QThread>
#include <QTextStream>
#include <QDebug>
#include <QPair>
#include <QList>
#include <QDir>
#include <instrucciones.h>
#include <QTimer>
#include <QPainter>


#define CANT_COLORES 72

enum{
    AMARILLO = 0,
    AZUL,
    AZULOSCURO,
    BLANCO,
    BORDO,
    CIAN,
    CIANOSCURO,
    GRISCLARO,
    GRISOSCURO,
    MAGENTA,
    MARRON,
    NEGRO,
    NARANJA,
    OLIVA,
    PURPURA,
    ROJO,
    ROSA,
    VERDE,
    VERDEAMARILLO,
    VERDEOSCURO
};

struct coordenadas_t {
    unsigned short x;
    unsigned char y;

    bool operator==(const coordenadas_t& other) const {
        return (x == other.x) && (y == other.y);
    }
};

typedef QVector<coordenadas_t> Matriz;

typedef struct{
    Matriz pixel;
    int cant;
    uint8_t color;
    uint16_t color565;
} matrizInfo_t ;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Crear una lista de listas para almacenar los valores CSV

    matrizInfo_t matriz[CANT_COLORES];

    // Colores predefinidos
    QList<QColor> coloresPredefinidos;
    QMap<QString, QString> mapaColores;

    QImage image;
    QImage imagenContorno;
    QImage imagenBordado;

    int cantComandosImagen = 0;

    bool envio = false;
    int fila = 0;
    int porcentajeBordado = 0;
    QList<QList<QString>> csvData;

    bool envioDisplay = false;
    int filaDisplay = 0;
    int porcentajeDisplay = 0;
    QList<QString> datosDisplay;

    bool envioVacio = false;

    QTimer *tiempoBordado = new QTimer(this);

    int indiceLinea = 0;

    bool mostrarEnviar = false;

private slots:

    void procesarImagen(QString fn, bool mostrarImagen);
    void procesarCsv(QString filePath);
    void mostrarBordado(void);
    void mostrarBordadoCompleto(void);

    /*
     * Función: limpiarMatriz
     * -----------------------
     * Limpia todos las matrices
     */
    void limpiarMatriz();

    /*
     * Función: limpiarMatriz
     * -----------------------
     * Limpia la matriz ingresada
     *
     * Parámetros:
     * matriz: matriz a limpiar
     */
    void limpiarMatriz(matrizInfo_t &matriz);

    /*
     * Función: setearColores
     * -----------------------
     * Añade los valores a la QList (para imagen)
     */
    void setearColores();

    /*
     * Función: setearColores
     * -----------------------
     * Añade los valores a la QList (para csv)
     */
    void setearColoresCsv();

    /*
     * Función: recorrerImagen
     * ------------------------
     * Recorre la imagen pixel a pixel y guarda las coordenadas del pixel en la matriz que corresponde.
     *
     * Parámetros:
     * imagen: imagen a escanear.
     *
     * returns: nada.
     */
    void recorrerImagen(QImage imagen);

    /*
     * Función: pintaMatrizEnImagen
     * -----------------------------
     * Funcion que crea una imagen pixel a pixel segun el color.
     *
     * Parametros:
     * imagen: imagen en donde recrear la imagen.
     * matriz: matriz del color del que se va a hacer.
     *
     * returns: nada.
     */
    void pintaMatrizEnImagen(QImage &imagen, matrizInfo_t &matriz);

    /*
     * Función: escalarImagenMin
     * -----------------------
     * Excala una imagen respetando las proporciones.
     *
     * Parametros:
     * maxAncho: maximo ancho de la imagen.
     * maxAlto: maximo alto de la imagen.
     * imagen: imagen a escalar.
     *
     * returns: matrizContorno.
     */
    QImage escalarImagenMin(int maxAncho, int maxAlto, QImage &imagen);

    /*
     * Función: escalarImagenMax
     * -----------------------
     * Excala una imagen respetando las proporciones.
     *
     * Parametros:
     * minAncho: minimo ancho de la imagen.
     * minAlto: minimo alto de la imagen.
     * imagen: imagen a escalar.
     *
     * returns: matrizContorno.
     */
    QImage escalarImagenMax(int minAncho, int minAlto, QImage &imagen);

    /*
     * Función: detectarPuntosdeInflexion
     * -----------------------
     * Detecta cuando en "x" un pixel cambia de color
     *
     * Parametros:
     * input: matriz de un color
     *
     * returns: matrizInfo_t con los puntos de inflexion
     */
    matrizInfo_t detectarPuntosDeInflexion(matrizInfo_t &matriz);

    /*
     * Función: encontrarColorMasCercano
     * ----------------------------------
     * Encuentra el color más cernano al valor RGB del pixel.
     *
     * Parametros:
     * color: almacena la cantidad de pixeles y las coordenadas de cada uno.
     *
     * returns: matrizContorno.
     */
    QColor encontrarColorMasCercano(const QColor& color, const QList<QColor>& colores);

    /*
     * Función: encontrarColorMasCercano
     * ----------------------------------
     * Función para calcular la distancia euclidiana entre dos colores.
     *
     * Parametros:
     * color1: color 1 a comparar con color2.
     * color2: color 2 a comparar con color1.
     *
     * returns: distancia.
     */
    double calcularDistancia(const QColor& color1, const QColor& color2);

    /*
     * Función: enviarFigura
     * ------------------------
     * Envía el comando de los pixeles de inicio y fin de una linea recta de la figura a través de UART 115200.
     *
     * Parametros:
     * matriz: figura.
     *
     * returns: nada.
     */
    void enviarFigura();

    void on_pbActualizar_clicked();

    void on_pbConectar_clicked();

    void on_pbEnviar_clicked();

    void on_pushButton_2_clicked();

    void on_datosRecibidos();

    void on_pushButton_3_clicked();

    void resetButtonStyle();

    void on_pushButton_clicked();

protected:
    void resizeEvent(QResizeEvent* evt) override;

private:

    // Definir las dimensiones máximas deseadas
    int maxAncho = 320;
    int maxAlto = 240;

    int minAncho = 320;
    int minAlto = 240;

    Ui::MainWindow *ui;
    void enumerarPuertos();

    QSerialPort puerto;

};
#endif // MAINWINDOW_H
