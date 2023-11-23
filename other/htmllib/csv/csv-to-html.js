$(document).ready(function() {
  // 读取csv数据
  var csvData = localStorage.getItem("csvData");
  if (csvData) {
    data = csvToArray(csvData);
  } else {
    data = [
      ["Name", "Age", "Gender"],
      ["John Smith", "25", "Male"],
      ["Jane Doe", "30", "Female"],
      ["Bob Johnson", "40", "Male"]
    ];

    // 初始化本地存储
     var csvString = "";
     for (var i=0; i<data.length;i++){
         csvString+=data[i].join(",")+"\n";
     }
     localStorage.setItem("csvData",csvString);
  }

  // 渲染表格
  renderTable(data);

  // 监听表格修改事件并同步到csv文件
  $("#table-container").on("input", "td[contenteditable='true']", function() {
    var row = $(this).parent().index();
    var col = $(this).index();
    var value = $(this).text();

    // 更新数据
    data[row][col] = value;

    // 同步到csv文件
     var csvString = "";
     for (var i=0; i<data.length;i++){
         csvString+=data[i].join(",")+"\n";
     }
     localStorage.setItem("csvData",csvString);
  });
});

function csvToArray(csv) {
  var rows = csv.split("\n");
  var result = [];
  for (var i = 0; i < rows.length; i++) {
    var row = rows[i].split(",");
    result.push(row);
  }
  return result;
}

function renderTable(data) {
  var html = "<table>";
  for (var i = 0; i < data.length; i++) {
    html += "<tr>";
    for (var j = 0; j < data[i].length; j++) {
      html += "<td contenteditable='true'>" + data[i][j] + "</td>";
    }
    html += "</tr>";
  }
  html += "</table>";
  $("#table-container").html(html);
}

