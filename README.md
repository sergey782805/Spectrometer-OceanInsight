# Spectrometer App (Qt / C++)

[English](#english) | [Русский](#русский)

---

## Русский

Десктопное приложение для визуализации и спектрального анализа световой среды, разработанное на C++ с использованием фреймворка Qt и среды Visual Studio. Предназначено для точного измерения и обработки параметров освещения.

### Основные возможности
* **Расчет параметров квантового потока**: вычисление важнейших характеристик фотосинтетически активной радиации — **PFD** (Photon Flux Density) в диапазоне 350-800 нм и **PPFD** (Photosynthetic Photon Flux Density) в диапазоне 400-700 нм. Точность и диапазон расчетов зависит от калибровочных коэффициентов.
* **Гибкая настройка спектрометра**: поддержка загрузки и применения уже готовых калибровочных коэффициентов для точной настройки измерений.
* **Визуализация спектра**: отображение графиков интенсивности и спектральных зависимостей в реальном времени.
* **Экспорт данных**: сохранение результатов анализа спектра в формат `.csv` для дальнейшей обработки и построения графиков.
* **Интерфейс на Qt**: отзывчивый пользовательский интерфейс, совмещающий классические элементы QtWidgets и QML.

### Требования для сборки
* **C++ Компилятор**: поддержка стандарта C++17 или новее.
* **IDE**: Visual Studio 2019 / 2022 (с установленным расширением Qt VS Tools).
* **Фреймворк**: Qt 6.11 (или совместимая версия).
* **Внешние зависимости**: установленный **OceanDirect SDK** (драйверы и библиотеки от Ocean Insight для работы со спектрометрами).

### Быстрый запуск
1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/sergey782805/Spectrometer-OceanInsight
   ```
2. Убедитесь, что на вашем компьютере установлен **OceanDirect SDK**, а пути к его заголовочным файлам (`include`) и библиотекам (`lib`) корректно прописаны в настройках Visual Studio.
3. Откройте файл проекта `QtWidgetsApplication1.vcxproj` в Visual Studio.
4. Проверьте и при необходимости настройте пути к вашей установленной версии Qt в настройках проекта.
5. Соберите и запустите проект (конфигурация Release или Debug).

---

## English

A desktop application for spectral data visualization and analysis, built with C++ using the Qt framework and Visual Studio. Designed for precise lighting measurement and processing.

### Key Features
* **Quantum Flux Calculations**: precise evaluation of core photosynthetically active radiation parameters — **PFD** (Photon Flux Density) and **PPFD** (Photosynthetic Photon Flux Density).
* **Flexible Device Calibration**: support for loading and applying pre-defined calibration coefficients for accurate measurements.
* **Spectrum Visualization**: real-time plotting of intensity and frequency dependencies.
* **Data Export**: saving analysis results directly into `.csv` format for external processing and graphing.
* **Qt UI**: responsive user interface using QtWidgets combined with QML components.

### Requirements
* **C++ Compiler**: C++17 support or newer.
* **IDE**: Visual Studio 2019 / 2022 (with Qt VS Tools extension installed).
* **Framework**: Qt 6.11 (or compatible version).
* **External Dependencies**: Installed **OceanDirect SDK** (drivers and libraries by Ocean Insight for spectrometer connectivity).

### Quick Start
1. Clone the repository:
   ```bash
   git clone https://github.com
   ```
2. Ensure that **OceanDirect SDK** is installed on your system, and its header (`include`) and library (`lib`) paths are properly configured in Visual Studio.
3. Open the `QtWidgetsApplication1.vcxproj` project file in Visual Studio.
4. Verify and update the local Qt paths in the project settings if necessary.
5. Build and run the project (Release or Debug configuration).

