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
    $scope.my_markdown = '## welcome to markdown file brewer\n' +
        '\n' +
        '\n' +
        '\n' +
        'the following files written by markdown format,and those will be show up on website by html format.\n' +
        '* math format:\n' +
        "$$ y(x,t) = y = A \\sin(kx - \\omega t) $$\n" +
        '* code format:\n' +
        '```c\n' +
        '#include <stdio.h>\n' +
        '```'
    function enableTab(id) {
        var el = document.getElementById(id);
        el.addEventListener('input', function(e) {
                // alert(this.value)
                var val = this.value,
                    start = this.selectionStart,
                    end = this.selectionEnd;
                this.value = val.substring(0, start) + '\t' + val.substring(end);

                this.selectionStart = this.selectionEnd = start + 1;

                return false;

        })
    };
    enableTab('txt')
})
