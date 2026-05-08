// LineGraph.qml
import QtGraphs
import QtQuick
import QtQuick.Shapes

 
GraphsView {
    
    Connections{ // NOT NEEDET AT ALL
        target: myModel
        
        function onDataChanged ()
        {
            series.clear()

            //console.log("CLICKED!")
            //console.log("X AFTER CLICK IS " + myModel.getMaxX)
            
            var data = myModel.getData
            //console.log("Data size in qml is " + data.length + "\n First point is " + data[0])
            //console.log("second point is " + data[1])
            //series.append(data)
            series.replace(data)
            //for(var i = 0; i < data.length; i++){
              //  series.append(data[i])
            //}

        }

    }

    anchors.fill: parent
    anchors.margins: 0
    
    theme: GraphsTheme 
    {
        colorScheme: GraphsTheme.ColorScheme.Dark
        seriesColors: ["#2CDE85", "#DBEB00"] 
    }

    axisX: ValueAxis 
    {
        id: axisX
        min: 0.0
        max: 1100.0//myModel.getMaxX //from Module
        tickInterval: 50.0
        labelDecimals: -1
        titleText: "nm"
        
    }

    axisY: ValueAxis 
    {
        min: 0.0
        max: 1.0 //myModel.getMaxY // from module
        tickInterval: 0.5
        labelDecimals: -1
        titleText: "Intensity"
    }

    //from module 
    AreaSeries 
    {
        
        //id: series
        //XYPoint { x: 0; y: 0.1 }
        //XYPoint { x: 1; y: 0.5 }
        upperSeries:LineSeries 
        {
            id: series
        }
        
        //gradient: LinearGradient {
        
        //x1: 0
        //y1: 0
        //x2: parent.width  // Конечная точка по горизонтали (правый край)
        //y2: 0 
        //purple
        //GradientStop { position: 0.345; color: "#8B00FF" }
        //GradientStop { position: 0.409; color: "#8B00FF" }
        //blue
        //GradientStop { position: 0.409; color: "blue" }
        //GradientStop { position: 0.4409; color: "blue" }
        //cyan
        //GradientStop { position: 0.4409; color: "cyan" }
        //GradientStop { position: 0.4545; color: "cyan" }
        //green
        //GradientStop { position: 0.4545; color: "green" }
        //GradientStop { position: 0.5136; color: "green" }
        //yellow
        //GradientStop { position: 0.5136; color: "yellow" }
        //GradientStop { position: 0.536; color: "yellow" }
        //orange
        //GradientStop { position: 0.536; color: "orange" }
        //GradientStop { position: 0.5681; color: "orange" }
        //red
        //GradientStop { position: 0.5681; color: "red" }
        //GradientStop { position: 0.709; color: "red" }

    //}


        

    }
    
 
    //series.remo
  
   

 
    
}