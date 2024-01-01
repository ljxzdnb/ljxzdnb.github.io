async function askChatGPT() {
  const userInput = document.getElementById('user-input').value;
  const responseElement = document.getElementById('markdown');

  try {
    // Replace 'YOUR_API_KEY' with your actual OpenAI API key
    const apiKey = 'sk-lKaqlIhrQfB0kFzXVOFzU61j3FEM0WFHZLQNdCQy4yJid2lI';
    const apiUrl = 'https://api.chatanywhere.com.cn/v1/chat/completions';

    const messages = [
      { role: 'user', content: userInput }
    ];

    const response = await fetch(apiUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${apiKey}`
      },
      body: JSON.stringify({
        model: 'gpt-4',
        messages: messages,
      })
    });

    const responseData = await response.json();
    const modelOutput = responseData.choices[0].message.content;
    var el = document.getElementById('txt');
    el.value = modelOutput;
    el.dispatchEvent(new Event('input',{bubbles:true}))
    // responseElement.textContent = `ChatGPT's response: ${modelOutput}`;
    MathJax.Hub.Typeset();
  } catch (error) {
    responseElement.textContent = 'Error communicating with ChatGPT API';
    console.error(error);
  }
}

