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
    $scope.my_markdown = '## introduction:\n' +
        '\n' +
        '* of this really fast introduction, my name is ji xuan Liao, I’ll be going ahead and giving you a little about my background.I’ve been working in an embedded space for forever probably 3 years or so, um and I started working with embedded linux in 2022.\n' +
        '\n' +
        '* since graduated from university,I enter a company of embedded system developing, the main work is suit our driver to the device, and cooperate with other department workmates to maintain our system work normally,And also develop a serial of test tools about stress test, code cover test,function run time test, and network traffic test. We just get the result and put it on dashboard for other workmates to optimize their code;\n' +
        '\n' +
        '* In fotile, I pay more attention on network, we use wpa to connect wifi and dhcp to get a ip address. once the board connected, opening a app on my phone that tell me which machine is mine, and send a message though it. the machine will receive correct information and decode it and deliver this command to the mcu by binary code.I also maked a program help us to burn our kernel image and file system into the board, and we can get informations by it’s function of send message to our website though network. it will be show up on dashboard of these msgs about the status of board, such as cpu usage, memory remaining, disk space.so it’s very convenient to user get the status of board.'
    //     Tab Support for Text-area
    function enableTab(id) {
        var el = document.getElementById(id);
        el.onkeydown = function(e) {
            if (e.keyCode === 9) {
                var val = this.value,
                    start = this.selectionStart,
                    end = this.selectionEnd;
                this.value = val.substring(0, start) + '\t' + val.substring(end);

                this.selectionStart = this.selectionEnd = start + 1;

                return false;

            }
        };
    };
    enableTab('txt')
})
