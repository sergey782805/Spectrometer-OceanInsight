// LineGraph.qml
import QtGraphs
import QtQuick
import QtQuick.Controls
import CustomTypes 1.0

 
GraphsView {
    
    id: graphsID

    //property GraphModel myModel: GraphModel {}

    GraphModel 
    {
        id: myModel
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
        min: 100.0
        max: myModel.getMaxX //from Module
        tickInterval: 100
        labelDecimals: -1
        titleText: "nm"
        
    }

    axisY: ValueAxis 
    {
        min: -5
        max: myModel.getMaxY // from module
        tickInterval: 10
        labelDecimals: -1
        titleText: "Intensity"
    }


    //from module 
    SplineSeries 
    {

        id: series
        
        
        //XYPoint { x: 0; y: 5 }
        //XYPoint { x: 1; y: 2 }
        
        //mySeries.append(
    }
    
    
     Component.onCompleted: {
        series.clear()
        
        var data = myModel.qList       
        //var maxY = myModel. 
        
         for (var i = 0; i < data.length; i++) 
            {
	            series.append(data[i])
            }

        //series.append(data)
        
        //series.append(myModel.qList)

    }
 
    //series.remo
  


    Connections
    {
        target: myModel
        function onMaxXChanged() {
        
            axisX.titleText = "Changed"    
            
        
        }
        

    }
   

    

    
}