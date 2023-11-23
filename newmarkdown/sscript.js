// $(document).ready(function() {
$('html').on('DOMMouseScroll mousewheel', function (e) {
  if(e.originalEvent.detail > 0 || e.originalEvent.wheelDelta < 0) { //alternative options for wheelData: wheelDeltaX & wheelDeltaY
    //scroll down
    console.log('Down');
    $( "#header-nav" ).addClass( "hide-nav-bar" );
  } else {
    //scroll up
    console.log('Up');
    $( "#header-nav" ).removeClass( "hide-nav-bar" );
  }
  //prevent page fom scrolling
  //return false;
});
// });

// On click show menu on small screen

//   $('body').addClass('js');
//   var $menu = $('#menu'),
//       $menulink = $('.menu-link');
//
// $menulink.click(function() {
//   $menulink.toggleClass('active');
//   $menu.toggleClass('active');
//   return false;
// });

// var toggled = 0;

// $('.menu-link').click(function(){
//   if(toggled == 0){
//   $('.bar3').stop().transition({rotate: "45", "margin-top": "13px"});
//   $('.bar2').stop().transition({opacity: "0"}, "fast");
//   $('.bar1').stop().transition({rotate: "-45", "margin-top": "13px"});
//     toggled++;
//     console.log("toggled down")
//   }
//   else{
    
//   $('.bar3').stop().transition({rotate: "+=135", "margin-top": "3px"});
//   $('.bar2').transition({opacity: "1"}, "fast");
//   $('.bar1').stop().transition({rotate: "-=135", "margin-top": "23px"});
//   toggled--;
//    console.log("Togged Up")
//   }
// });

// var tabsVerticalInner = $('#menu');
// var selectorVerticalInner = $('#menu').find('li').length;
// var activeItemVerticalInner = tabsVerticalInner.find('.active');
// var activeWidthVerticalHeight = activeItemVerticalInner.innerHeight();
// var activeWidthVerticalWidth = activeItemVerticalInner.innerWidth();
// var itemPosVerticalTop = activeItemVerticalInner.position();
// var itemPosVerticalLeft = activeItemVerticalInner.position();

$("#menu").on("click","a",function (){
  var file = $(this).text();
  var el = document.getElementById('txt');
  var xhr = new XMLHttpRequest();
  xhr.open('GET','./files/'+file+'.md',true);
  xhr.readyState = 'text';
  xhr.onload = function (){
    if(xhr.status === 200){
      // $scope.my_markdown = xhr.responseText;
      el.value = xhr.responseText;
      el.dispatchEvent(new Event('input',{bubbles:true}))
      MathJax.Hub.Typeset();
    }
  }
  xhr.send();
});
// $("#menu").on("click","li",function(e){
//   $('#m ul li').removeClass("active");
//   $(this).addClass('active');
//   var activeWidthVerticalHeight = $(this).innerHeight();
//   var activeWidthVerticalWidth = $(this).innerWidth();
//   var itemPosVerticalTop = $(this).position();
//   var itemPosVerticalLeft = $(this).position();
//   $(".selector-active").css({
//     "top":itemPosVerticalTop.top + "px",
//     "left":itemPosVerticalLeft.left + "px",
//     "height": activeWidthVerticalHeight + "px",
//     "width": activeWidthVerticalWidth + "px"
//   });
// });