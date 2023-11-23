// 定义一个表示C语言结构体的JavaScript类
class CStruct {
  // 构造函数，接受一个二进制数据缓冲区作为参数
  constructor(buffer) {
    // 创建DataView实例以便于读取二进制数据
    this.dataView = new DataView(buffer);
    // 解析二进制数据并赋值给类属性
    this.id = this.dataView.getInt32(0, true); // 第1个4字节存储ID
    this.name = this.getStringFromBuffer(4, 20); // 从第5个字节开始的20个字节存储名字，使用自定义方法getStringFromBuffer进行解析
    this.age = this.dataView.getInt16(24, true); // 第25个和第26个字节存储年龄
  }

  // 从缓冲区中截取一段字节，并将其转换为字符串
  getStringFromBuffer(offset, length) {
    const stringBuffer = [];
    for (let i = offset; i < offset + length; i++) {
      stringBuffer.push(this.dataView.getUint8(i));
    }
    return String.fromCharCode.apply(null, stringBuffer);
  }
}

// 定义一个函数，用于读取二进制文件
function readBinaryFile(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = () => {
      resolve(reader.result);
    };
    reader.onerror = () => {
      reject(reader.error);
    };
    reader.readAsArrayBuffer(file);
  });
}

// 从文件选择器中获取要读取的二进制文件
const input = document.createElement('input');
input.type = 'file';
input.addEventListener('change', async (event) => {
  const file = event.target.files[0];
  if (!file) {
    return;
  }
  try {
    // 读取二进制文件
    const binaryData = await readBinaryFile(file);
    // 解析二进制数据为CStruct对象
    const cStruct = new CStruct(binaryData);
    console.log(cStruct);
  } catch (error) {
    console.error(error);
  }
});
document.body.appendChild(input);

