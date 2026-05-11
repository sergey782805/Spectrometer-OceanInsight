// LineGraph.qml
pragma ComponentBehavior: Bound
import QtGraphs
import QtQuick
import QtQuick.Shapes

 
GraphsView {
    
    id: graphView    

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

    //anchors.fill: parent
    //anchors.margins: 0
    
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
    LineSeries 
    {
        id: series
        //id: series
        //XYPoint { x: 0; y: 0.1 }
        //XYPoint { x: 1; y: 0.5 }
        //upperSeries:LineSeries 
        //{
        //    id: series
        //}
        //gradient: LinearGradient {
        
        //x2: parent.width

    //GradientStop { position: 0.0; color: "black" }
    //GradientStop { position: 0.228; color: "#8B00FF" } // 380 нм
    //GradientStop { position: 0.308; color: "blue" }    // 450 нм
    //GradientStop { position: 0.365; color: "cyan" }    // 500 нм
    //GradientStop { position: 0.439; color: "green" }   // 565 нм
    //GradientStop { position: 0.467; color: "yellow" }  // 590 нм
    //GradientStop { position: 0.507; color: "orange" }  // 625 нм
    //GradientStop { position: 0.683; color: "red" }     // 780 нм
    //GradientStop { position: 0.75; color: "black" }
    //GradientStop { position: 1.0; color: "black" }

    }


        

    }
    
 
    //series.remo
  
  