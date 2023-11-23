function test(){
	var tabsNewAnim = $('#navbarSupportedContent');
	var selectorNewAnim = $('#navbarSupportedContent').find('li').length;
	var activeItemNewAnim = tabsNewAnim.find('.active');
	var activeWidthNewAnimHeight = activeItemNewAnim.innerHeight();
	var activeWidthNewAnimWidth = activeItemNewAnim.innerWidth();
	var itemPosNewAnimTop = activeItemNewAnim.position();
	var itemPosNewAnimLeft = activeItemNewAnim.position();
	$(".hori-selector").css({
		"top":itemPosNewAnimTop.top + "px",
		"left":itemPosNewAnimLeft.left + "px",
		"height": activeWidthNewAnimHeight + "px",
		"width": activeWidthNewAnimWidth + "px"
	});
	$("#navbarSupportedContent").on("click","a",function (){
		var linkId = $(this).attr("id");
		$("#content iframe").hide();
		$("#"+linkId.replace("nav","content")).show();
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
	$("#navbarSupportedContent").on("click","li",function(e){
		$('#navbarSupportedContent ul li').removeClass("active");
		$(this).addClass('active');
		var activeWidthNewAnimHeight = $(this).innerHeight();
		var activeWidthNewAnimWidth = $(this).innerWidth();
		var itemPosNewAnimTop = $(this).position();
		var itemPosNewAnimLeft = $(this).position();
		$(".hori-selector").css({
			"top":itemPosNewAnimTop.top + "px",
			"left":itemPosNewAnimLeft.left + "px",
			"height": activeWidthNewAnimHeight + "px",
			"width": activeWidthNewAnimWidth + "px"
		});
	});
}
$(document).ready(function(){
		$('html').on('DOMMouseScroll mousewheel', function (e) {
  	if(e.originalEvent.detail > 0 || e.originalEvent.wheelDelta < 0) { //alternative options for wheelData: wheelDeltaX & wheelDeltaY
    //scroll down
    console.log('Down');
    $( ".navbar-mainbg" ).addClass( "hide-nav-bar" );
  	} else {
    //scroll up
    console.log('Up');
    $( ".navbar-mainbg" ).removeClass( "hide-nav-bar" );
  	}
  	//prevent page fom scrolling
  	//return false;
	});
	setTimeout(function(){ test(); });
});
$(window).on('resize', function(){
	setTimeout(function(){ test(); }, 500);
});
$(".navbar-toggler").click(function(){
	$(".navbar-collapse").slideToggle(300);
	setTimeout(function(){ test(); });
});



// --------------add active class-on another-page move----------
jQuery(document).ready(function($){
	// Get current path and find target link
	var path = window.location.pathname.split("/").pop();

	// Account for home page with empty path
	if ( path == '' ) {
		path = 'index.html';
	}

	var target = $('#navbarSupportedContent ul li a[href="'+path+'"]');
	// Add active class to target link
	target.parent().addClass('active');
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