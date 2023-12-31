angular.module('app', ['hc.marked'])

angular.module('app').config(function(markedProvider) {
    markedProvider.setOptions({
        gfm: true,
        tables: true,
        highlight: function(code) {
            return hljs.highlightAuto(code).value;
        }
    });
})

angular.module('app').controller('ctrl', function($scope) {
    $scope.my_markdown = '## *welcome to <u>markdown</u> file ~~brewer~~*\n' +
        '\n' +
        '> the following files written by markdown format,and those will be show up on website by html format.\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **header**\n' +
        '\n' +
        '1. # header\n' +
        '\n' +
        '2. ## header\n' +
        '\n' +
        '3. ### header\n' +
        '\n' +
        '4. #### header\n' +
        '\n' +
        '5. ##### header\n' +
        '\n' +
        '6. ###### header\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **math format**:\n' +
        '  $$ y(x,t) = y = A \\sin(kx - \\omega t) $$\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **code format**:\n' +
        '\n' +
        '```c\n' +
        '#include <stdio.h>\n' +
        '\n' +
        'int main(){\n' +
        '    printf("ljxzdnb!!!");\n' +
        '}\n' +
        '```\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **colorfull line**:\n' +
        '\n' +
        '  <font color=\'red\'>this is test line!</font><br><font color=\'blue\'>this is test line!!</font>\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **images**:\n' +
        '\n' +
        '![](./wechat.jpg)\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **tables**:\n' +
        '\n' +
        '| A1   | B1   | C1   |\n' +
        '| ---- | ---- | ---- |\n' +
        '| A2   | B2   | C2   |\n' +
        '| A3   | B3   | C3   |\n' +
        '| A4   | B4   | C4   |\n' +
        '\n' +
        '---\n' +
        '\n' +
        '* **hyperlink**:\n' +
        '\n' +
        '[baidu](www.baidu.com)\n' +
        '\n' +
        '\n' +
        '\n' +
        '<br>'
    function enableTab(id) {
        var el = document.getElementById(id);
        el.addEventListener('input', function(e) {
                // alert(this.value)
                var val = this.value,
                start = this.selectionStart,
                end = this.selectionEnd;
                this.value = val.substring(0, start) + '\t' + val.substring(end);
                this.selectionStart = this.selectionEnd = start + 1;
        })
    };
    enableTab('txt')
})
