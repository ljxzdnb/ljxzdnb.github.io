// ---------vertical-menu with-inner-menu-active-animation-----------

var tabsVerticalInner = $('#accordian');
var selectorVerticalInner = $('#accordian').find('li').length;
var activeItemVerticalInner = tabsVerticalInner.find('.active');
var activeWidthVerticalHeight = activeItemVerticalInner.innerHeight();
var activeWidthVerticalWidth = activeItemVerticalInner.innerWidth();
var itemPosVerticalTop = activeItemVerticalInner.position();
var itemPosVerticalLeft = activeItemVerticalInner.position();
$(".selector-active").css({
	// "top":itemPosVerticalTop.top + "px",
	// "left":itemPosVerticalLeft.left + "px",
	"height": 45 + "px",
	// "width": activeWidthVerticalWidth + "px"
});
$("#accordian").on("click","a",function (){
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
$("#accordian").on("click","li",function(e){
	$('#accordian ul li').removeClass("active");
	$(this).addClass('active');
	var activeWidthVerticalHeight = $(this).innerHeight();
	var activeWidthVerticalWidth = $(this).innerWidth();
	var itemPosVerticalTop = $(this).position();
	var itemPosVerticalLeft = $(this).position();
	$(".selector-active").css({
		"top":itemPosVerticalTop.top + "px", 
		"left":itemPosVerticalLeft.left + "px",
		"height": activeWidthVerticalHeight + "px",
		"width": activeWidthVerticalWidth + "px"
	});
});


// --------------add active class-on another-page move----------
// jQuery(document).ready(function($){
//   //Get current path and find target link
//   var path = window.location.pathname.split("/").pop();
//
//   //Account for home page with empty path
//   if ( path == '' ) {
//     path = 'index.html';
//   }
//
//   var target = $('#accordian ul li a[href="'+path+'"]');
//   // Add active class to target link
//   target.parent().addClass('active');
// });