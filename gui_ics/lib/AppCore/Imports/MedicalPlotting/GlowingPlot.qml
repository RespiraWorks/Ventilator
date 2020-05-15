import QtQuick 2.0
import QtGraphicalEffects 1.0
import com.ics.waveformchart 1.0

//PREVIEW IMAGE qrc:/MedicalPlottingComponentSet/PreviewAssets/icon.png
Item {
    id: root

    width: 256
    height: 128

    //COLOR plotColor
    property color plotColor: "#89c435"
    //REAL 2.0
    property alias waveformWidth: wave.waveformWidth
    property bool dottedLine: false
    property int waveformType: 0
    property int waveformPlotType: 0

    property string dataPath: ":/MedicalPlottingDemoData/b0_3s.txt"

    property int plotScale: 2800
    property int glowRadius: 12
    property real glowOpacity: 0.5

    Item {
        id: waveSrc

        anchors.fill: parent

        DropShadow {
            anchors.fill: wave

            samples: 8
            radius: root.glowRadius
            opacity: root.glowOpacity
            color: wave.waveformColor
            source: wave
        }

        WaveformChartWidget {
            id: wave

            anchors.fill: parent
            anchors.leftMargin: -20

            autoScale: false
            autoSize:  false

            titleColor: "transparent"

            gridColor: "transparent"
            gridLabelsColor: "transparent"
            showVerticalGrid: false
            showVerticalGridLabels: false

            waveformType: root.waveformType
            waveformPlotType: root.waveformPlotType
            waveformColor: root.plotColor
            waveformStyle: root.dottedLine ? Qt.DotLine : Qt.SolidLine
            waveformWidth: 2.0

            selected: false

            Component.onCompleted: {
                dataPath = root.dataPath
                init(0)
                sweepSpeed = 50
                scale = root.plotScale
            }

            onWidthChanged: updateSize()
            onHeightChanged: updateSize()
        }
    }

    ShaderEffectSource {
        id: shaderSrc

        sourceItem: waveSrc
        hideSource: true
    }

    property real leftFadeStart: 0.0477
    Rectangle {
        id: mask

        width: parent.height
        height: parent.width
        anchors.centerIn: parent

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "transparent"
            }
            GradientStop {
                position: root.leftFadeStart
                color: "white"
            }
        }
    }

    ShaderEffectSource {
        id: maskSrc

        sourceItem: mask
        hideSource: true
    }

    ShaderEffect {
        id: fadedWave

        property var src: shaderSrc
        property var mask: maskSrc

        anchors.fill: parent
        vertexShader: "
        uniform highp mat4 qt_Matrix;
        attribute highp vec4 qt_Vertex;
        attribute highp vec2 qt_MultiTexCoord0;

        varying highp vec2 coord;

        void main() {
            coord = qt_MultiTexCoord0;
            gl_Position = qt_Matrix * qt_Vertex;
        }"
        fragmentShader: "
            varying highp vec2 coord;
            uniform sampler2D src;
            uniform sampler2D mask;
                  uniform lowp float qt_Opacity;
            void main() {
                lowp vec4 tex = texture2D(src, coord);
                lowp vec4 maskSrc = texture2D(mask, coord.yx);
                gl_FragColor = tex * maskSrc.a * qt_Opacity;
            }"
    }

    onDataPathChanged: {
        wave.dataPath = dataPath
    }
    onPlotScaleChanged: {
        wave.scale = plotScale
    }
}
