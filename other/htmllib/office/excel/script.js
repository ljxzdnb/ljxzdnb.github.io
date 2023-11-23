document.querySelectorAll(".drop-zone__input").forEach((inputElement) => {
  const dropZoneElement = inputElement.closest(".drop-zone");

  dropZoneElement.addEventListener("click", (e) => {
    inputElement.click();
  });

  inputElement.addEventListener("change", (e) => {
    if (inputElement.files.length) {
      updateThumbnail(dropZoneElement, inputElement.files[0]);
    }
  });

  dropZoneElement.addEventListener("dragover", (e) => {
    e.preventDefault();
    dropZoneElement.classList.add("drop-zone--over");
  });

  ["dragleave", "dragend"].forEach((type) => {
    dropZoneElement.addEventListener(type, (e) => {
      dropZoneElement.classList.remove("drop-zone--over");
    });
  });

  dropZoneElement.addEventListener("drop", (e) => {
    e.preventDefault();

    if (e.dataTransfer.files.length) {
      inputElement.files = e.dataTransfer.files;
      updateThumbnail(dropZoneElement, e.dataTransfer.files[0]);
    }

    dropZoneElement.classList.remove("drop-zone--over");
  });
});

/**
 * Updates the thumbnail on a drop zone element.
 *
 * @param {HTMLElement} dropZoneElement
 * @param {File} file
 */
function updateThumbnail(dropZoneElement, file) {
  let thumbnailElement = dropZoneElement.querySelector(".drop-zone__thumb");

  // First time - remove the prompt
  if (dropZoneElement.querySelector(".drop-zone__prompt")) {
    dropZoneElement.querySelector(".drop-zone__prompt").remove();
  }

  // First time - there is no thumbnail element, so lets create it
  if (!thumbnailElement) {
    thumbnailElement = document.createElement("div");
    thumbnailElement.classList.add("drop-zone__thumb");
    dropZoneElement.appendChild(thumbnailElement);
  }

  thumbnailElement.dataset.label = file.name;

  // Show thumbnail for image files
  const reader = new FileReader();
  if (file.type.endsWith("xlsx")) {
    reader.readAsArrayBuffer(file);
    reader.onload = () => {
    var data = new Uint8Array(reader.result);
    var work_book = XLSX.read(data, {type:'array'});
    var sheet_name = work_book.SheetNames;
    var table_output = '<ul>';
    for(var sheet = 0; sheet < sheet_name.length; sheet++)
    {
        table_output += '<li><a>'+sheet_name[sheet]+'</a></li>'
    }
    table_output += '<li style="float:right"><a href="#top">top</a></li>'
    table_output += '</ul>';
    var sheet_data = XLSX.utils.sheet_to_json(work_book.Sheets[sheet_name[0]], {header:1});
    if(sheet_data.length > 0)
    {
        table_output += '<table>';
        for(var row = 0; row < sheet_data.length; row++)
        {
            if(row == 0){
                table_output += '<tr id="top"><th></th>';
            }
            else
            {
                table_output += '<tr>';
            }
            for(var cell = 0; cell < sheet_data[row].length; cell++)
            {
                if(row == 0)
                {
                    table_output += '<th>'+sheet_data[row][cell]+'</th>';
                }
                else if(row != 0 && cell == 0)
                {
                    table_output += '<td>'+ row +'</td><td>'+sheet_data[row][cell]+'</td>';
                }
                else
                {
                    table_output += '<td>'+sheet_data[row][cell]+'</td>';
                }
            }
            table_output += '</tr>';
        }
        table_output += '</table>';
//        const blob = new Blob([ table_output ], {type: "text/plain;charset=utf-8"})
//        const objectURL = URL.createObjectURL(blob)
//        const anchor = document.createElement('a')
//        anchor.href = objectURL
//        anchor.download = abs_path
//        anchor.click()
//        URL.revokeObjectURL(objectURL)
//        thumbnailElement.innerHTML = `<iframe src='${path}' frameborder="0">您的浏览器不支持iframe，请升级</iframe>`;
        thumbnailElement.innerHTML = table_output;
        }
    }
  } else {
    //注意：FormData只兼容到IE10
  
    alert(file.type);
    let xhr = new XMLHttpRequest();
    xhr.open('POST', "/", true);
    xhr.setRequestHeader("Accept", "application/x-bzip2,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    xhr.setRequestHeader('File-Name', file.name);
    xhr.setRequestHeader('Content-Type', file.type);
    xhr.send(file);
    reader.readAsDataURL(file)
    reader.onload = () => {
    thumbnailElement.innerHTML = `<iframe src='${reader.result}' frameborder="0">您的浏览器不支持iframe，请升级</iframe>`;
    }
  }
}
