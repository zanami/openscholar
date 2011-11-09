
//Load the Visualization API and the piechart package.
google.load('visualization', '1.1', {'packages':['controls']});


function drawChart() {
  var data = google.visualization.arrayToDataTable( Drupal.settings.openscholar_log_usage_data );
  
  var barChart = new google.visualization.ChartWrapper({
    'chartType': 'BarChart',
    'containerId': 'chart_div',
    'options': {
      'width': 800,
      'height': Drupal.settings.openscholar_log_usage_data.length * 30,
      'chartArea': {top: 0, right: 0, bottom: 0}
     },
    'view': {'columns': [0,1]}, //host, vsite //views = 3, but on different scale
  });
  
  var agePicker = new google.visualization.ControlWrapper({
    'controlType': 'NumberRangeFilter',
    'containerId': 'control-age',
    'options': {
      'filterColumnLabel': 'Age',
      'ui': {
        'labelStacking': 'vertical',
        'allowTyping': false,
        'allowMultiple': false,
      }
    }
  });

  var vsitesPicker = new google.visualization.ControlWrapper({
    'controlType': 'NumberRangeFilter',
    'containerId': 'control-vsites',
    'options': {
      'filterColumnLabel': 'Sites',
      'ui': {
        'labelStacking': 'vertical',
        'allowTyping': false,
        'allowMultiple': false,
      }
    }
  });
  // Create the dashboard.
  //new google.visualization.Dashboard(document.getElementById('dashboard')).bind(agePicker, barChart)).draw(data);   
  new google.visualization.Dashboard(document.getElementById('dashboard')).
  bind(agePicker, barChart).
  bind(vsitesPicker, barChart).
  draw(data); 
}


//Set a callback to run when the Google Visualization API is loaded.
google.setOnLoadCallback(drawChart);