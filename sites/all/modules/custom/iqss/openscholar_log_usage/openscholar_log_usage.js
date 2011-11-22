
//Load the Visualization API and the piechart package.
google.load('visualization', '1.1', {'packages':['controls','table']});


function drawChart() {

  var data = google.visualization.arrayToDataTable( Drupal.settings.openscholar_log_usage_data );

  //add date columns
  var col_age  = data.insertColumn(5, 'date', 'Created');
  var col_last = data.insertColumn(5, 'date', 'Visited', 'last');
  for (var row = 0 ; row < data.getNumberOfRows() ; row++) {
    data.setCell(  row, 5, new Date(1000*data.getValue(row, 4))  ); //1000 because php stores seconds but js does ms
    data.setCell(  row, 6,  new Date(1000*data.getValue(row, 2))  );
  }
  //hide old columns
  data.removeColumn(2);
  data.removeColumn(3);
  
  var tableChart = new google.visualization.ChartWrapper({
    'chartType': 'Table',
    'containerId': 'table_div',
    'options': {
      'showRowNumber': true,
      'allowHtml': true,
      'sortAscending': false,
      'sortColumn': 2,
      
    },
  });
  
  //set up some filters so we can add controls with slightly less copy/paste
  function RangeFilter() {
    return {
      'controlType': 'NumberRangeFilter',
      'options': {
        'ui': {
          'labelStacking': 'vertical',
          'allowTyping': false,
          'allowMultiple': false,
        }
      } 
    };
  }
  
  function StringFilter() {
    return {
      'controlType': 'StringFilter',
      'options': {
        'matchType': 'any',
      },
    };
  }
  
  function CategoryFilter() {
    return {
      'controlType': 'CategoryFilter',
      'options': {
        'filterColumnLabel': 'Metric',
        'ui': {
          'allowTyping': false,
          'allowMultiple': true,
          'selectedValuesLayout': 'belowStacked'
        }
      },
      // Define an initial state, i.e. a set of metrics to be initially selected.
      'state': {}
    };
  }
  
  //and add the filters to controllers
  
  //host doesn't quite filter correctly because the link is being matched.  will probalby have to set up an event to load the url instead
  var host = StringFilter();
  host.containerId = 'control-host';
  host.options.filterColumnLabel = 'Host';
  var hostPicker = new google.visualization.ControlWrapper(host);
  /*
  var age = RangeFilter();
  age.containerId = 'control-age';
  age.options.filterColumnLabel = 'Age';
  age.options.ui = {'label': 'Age (In Weeks)'};
  var agePicker = new google.visualization.ControlWrapper(age);
*/
  var vsites = RangeFilter();
  vsites.containerId = 'control-vsites';
  vsites.options.filterColumnLabel = 'Sites';
  var vsitesPicker = new google.visualization.ControlWrapper(vsites);
  
  var views = RangeFilter();
  views.containerId = 'control-views';
  views.options.filterColumnLabel = 'Views';
  var viewsPicker = new google.visualization.ControlWrapper(views);
  
  var version = CategoryFilter();
  version.containerId = 'control-version';
  version.options.filterColumnLabel = 'OS Version';
  var versionPicker = new google.visualization.ControlWrapper(version);

  //control-last_visit (would this be better served as a count than a date?
  
  //show dates as dates and not timestamps
  /*var mydata = Drupal.settings.openscholar_usage_data;
  for (var row in mydata) {
    mydata[row][2] = new Date(mydata[row][2]);
  }*/   //date and datetime columns are not supported
  /*
  var dateformatter = new google.visualization.DateFormat({ formatType:'Medium'});
  dateformatter.format(data, 2);
 */
  
  //try a pattern formatter for date
  //also for host, so we can keep original url for filter, but add an href
  
  //url formatter - makes hyperlink to each installation
  var url_formatter = new google.visualization.PatternFormat('<a target="_blank" href="http://{0}">{0}</a>');
  url_formatter.format(data, [0]);
  
  // Create the dashboard.
  //new google.visualization.Dashboard(document.getElementById('dashboard')).bind(agePicker, barChart)).draw(data);   
  new google.visualization.Dashboard(document.getElementById('dashboard')).
  bind([hostPicker, vsitesPicker, viewsPicker, versionPicker], [ tableChart]).
  //bind(agePicker, barChart).
  //bind(vsitesPicker, barChart).

  draw(data); 
}


//Set a callback to run when the Google Visualization API is loaded.
google.setOnLoadCallback(drawChart);