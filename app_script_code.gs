function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  var data;

  // Check if postData is available
  if (e && e.postData && e.postData.contents) {
    try {
      data = JSON.parse(e.postData.contents);
    } catch (error) {
      // If JSON parsing fails, use default values
      data = { ip: "0.0.0.0" }; // Default ip value
    }
  } else {
    // If postData is not available, use default values
    data = { ip: "0.0.0.0" }; // Default ip value
  }

  // Insert a new row at the top (before the first row)
  sheet.insertRowBefore(1);

  // Write the current timestamp and received data (or default value) to the first row
  var currentTime = new Date();
  sheet.getRange(1, 1).setValue(currentTime); // Timestamp in column A
  sheet.getRange(1, 2).setValue(data.ip);  // Data in column B

  // Delete rows older than a preset number of days
  deleteOldRows(sheet, 7); // Delete rows older than 7 days

  return ContentService.createTextOutput("Success");
}

function deleteOldRows(sheet, daysThreshold) {
  var data = sheet.getDataRange().getValues(); // Get all data in the sheet
  var currentTime = new Date();
  var rowsToDelete = [];

  // Loop through all rows (starting from the second row to skip the header)
  for (var i = 1; i < data.length; i++) {
    var rowDate = new Date(data[i][0]); // Assuming the timestamp is in column A
    var timeDifference = (currentTime - rowDate) / (1000 * 60 * 60 * 24); // Difference in days

    if (timeDifference > daysThreshold) {
      rowsToDelete.push(i + 1); // Rows are 1-indexed in Google Sheets
    }
  }

  // Delete rows in reverse order to avoid index shifting issues
  for (var j = rowsToDelete.length - 1; j >= 0; j--) {
    sheet.deleteRow(rowsToDelete[j]);
  }
}
