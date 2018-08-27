//array of page Categories
var bn_pageCategory = new Array();
var mktBNWidgets = {};

function bnMKVers() { return "global 1.17.2.6";}

function bnProduct()  {
    this.slot = "";
    this.ThumbUrl = "";
    this.SKU = "";
    this.ProductName = "";
    this.seoURL = "";
    this.Manufacturer = "";
    this.MfrPartNumber = "";
    this.UOM = "";
    this.Price = "";
    this.MinOrderQnty = "";
    this.ParentCategory = "";
    this.SubCategory = "";
    this.url = "";
}

function mapRec(recData) {
var ret = new bnProduct();
recData.attrs.forEach(function(obj) {ret[obj.name]=obj.values[0]} )
ret.slot = recData.slot;
ret.url = recData.url;
if (ret.seoURL === "") 
{
    ret.seoURL = ret.url;
}
return ret;
}

//add to cart function
function bn_AddToCartEvent() {
    var bn_submitted_form = this.form;
    if(bn_submitted_form != undefined && BaynoteAPI.isNotEmpty(bn_submitted_form)
            && BaynoteAPI.isNotEmpty(bn_submitted_form.product)){
        var bn_productId = bn_submitted_form.product.value;
        if (bn_productId != undefined && BaynoteAPI.isNotEmpty(bn_productId)){
            var exitInfo = new Object();
            if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
                exitInfo.attrs = exitInfo.attrs || {};
            }
                                
            exitInfo.attrs.action = "AddToCart";
            exitInfo.attrs.target = "http://" +
                location.hostname + 
                "/jsp/search/productdetail.jsp?SKU=" +
                bn_productId;
            bnObserver.exitOccurred (exitInfo);
        }
    }
    return true;
}

function computePageURL() {
    var head_tag = document.getElementsByTagName('head')[0];
    var link_tags = head_tag.getElementsByTagName('link');
    for ( var i = 0; i < link_tags.length; i++) {
        if (link_tags[i].getAttribute('rel').toLowerCase() == 'canonical') {
            var url = link_tags[i].getAttribute('href');
            if (url && location.search) {
                var params = location.search.substr(1).split("&");
                for (var j = 0; j < params.length; j++) {
                    var key_value = params[j].split("=");
                    if (key_value[0] == "bnec") {
                        url += url.indexOf("?") == -1 ? "?" : "&";
                        url += "bnec=" + key_value[1];
                    }
                }
            }
            return url;
        }
    }
    return window.location.href;
}

// exit handler
function bn_onClickHandler(clickedElement, exitInfo) {
    if (clickedElement == undefined)
        return false;

    if (typeof (bnObserver) != 'undefined'
            && typeof (bnObserver.defaultExitConfirmation) != 'undefined') {
        exitResult = bnObserver.defaultExitConfirmation(clickedElement,
                exitInfo);
        if (!exitResult)
            return false;
    }

    if (clickedElement) {
        if (exitInfo != null && typeof (exitInfo.attrs) == "undefined") {
            exitInfo.attrs = new Object();
        }
        while (clickedElement.tagName != "A" && clickedElement.tagName != "BODY") {
            clickedElement = clickedElement.parentNode;
        }
        if (clickedElement.className == "baynote btn btnSecondary btnSmall leftLoading ajaxAddToCart trolley refactored") {
            clickedElement = clickedElement.parentElement.parentElement.parentElement.getElementsByTagName("a")[0];
        }
        exitInfo.baynote_req = clickedElement.getAttribute("data-baynote-widget") || clickedElement.getAttribute("baynote_req");
        if (exitInfo.baynote_req) {
            exitInfo.baynote_guide = clickedElement.getAttribute("baynote_guide") || "DUMMY";
        }
        exitInfo.baynote_bnrank = clickedElement.getAttribute("data-baynote-slot") || clickedElement.getAttribute("baynote_bnrank");
        var clickProdId = clickedElement.getAttribute("data-baynote-pid") || clickedElement.getAttribute("baynote_pid");
        if (clickProdId) {
            exitInfo.attrs.prodId = clickProdId;
        }

        exitResult = true;
    }
    return exitResult;
}

function bn_AddShoppingCartProducts() {
	var bn_cartProducts = [];
	var productIds = '';
	var prodIds = document.getElementsByName('itemsArray');
	if (prodIds){ 
		productIds = prodIds[0].value;
	}
	if (productIds != '') {
		var prodArray = productIds.split(',');
		
		for(var i = 0; i < prodArray.length; i++) {
			bn_cartProducts.push("http://"+location.hostname+"/jsp/search/productdetail.jsp?SKU="+ prodArray[i]);
		}
	}
	return bn_cartProducts;
}

/** TODO - combine 'AddShoppingCartProducts and 'mer_AddPurchasedProducts' **/
function mer_AddPurchasedProducts(){
    var mer_purchasedProducts = [];
    var productIds = '';
    var prodIds = document.getElementById('taggingBasketItems');

    if (prodIds){
        productIds = prodIds.value;
    }
    if (productIds != '') {
        productIds = productIds.replace(/;/g,"");
        var prodArray = productIds.split(',');
        //console.log('prodArray.length',prodArray.length," ",prodArray);

        for(var i = 0; i < prodArray.length; i++) {
            mer_purchasedProducts.push("http://"+location.hostname+"/jsp/search/productdetail.jsp?SKU="+ prodArray[i]);
        }
    }
    return mer_purchasedProducts;
}

//manually pass in a "recent history" to the guide : set listUrl
//needed ?
function bn_ListShoppingCartProducts(tag){
    var bn_cartProducts = [];
    var re = new RegExp(".*/dp/([0-9A-Z]+).*");
    var orderDetails = document.getElementById("order_details");
    var a = orderDetails.getElementsByTagName("a");
    for (var i = 0; i < a.length; i++) {
        if (a[i].className == "bnSku") {
            var match = re.exec(a[i].href);
            if (match && match[1]) {
                bn_cartProducts.push("http://"+ window.location.host + "/jsp/search/productdetail.jsp?SKU=" + match[1]);
            }
        }
    }

    tag.url = "";
    tag.listUrls = bn_cartProducts;
}

/*
 * function for visitsTrail function to check if browser is IE 
 */
function isIE () {
  var myNav = navigator.userAgent.toLowerCase();
  return (myNav.indexOf('msie') != -1) ? parseInt(myNav.split('msie')[1]) : false;
}

/*
 * Add recently viewed products to trail
 */
function addToVisitsTrail(prodPage) {
	var bnTrail = BaynoteAPI.getCookieValue('bn_documentVisitsTrail');
    var date = new Date();
    date.setTime(date.getTime() + (28 * 24 * 60 * 60 * 1000));
    var expires = "; expires=" + date.toGMTString();
    if (bnTrail) {
        var bnTrailArray = new Array();
        bnTrailArray = bnCommon.parseJSON(bnTrail);
        // Index of currentURL in cookie array
        var urlIndex;
        if (isIE () < 9) {
            if (!Array.prototype.indexOf)
            {
                Array.prototype.indexOf = function(elt /*, from*/)
                {
                    var len = this.length;
                    var from = Number(arguments[1]) || 0;
                    from = (from < 0)
                         ? Math.ceil(from)
                         : Math.floor(from);
                    if (from < 0)
                      from += len;

                    for (; from < len; from++)
                        {
                      if (from in this && this[from] === elt)
                        return from;
                    }
                    return -1;
                  };
            }

            urlIndex = bnTrailArray.indexOf(prodPage);

        // is IE version less than 9
        } else {
         // is IE 9 and later or not IE
            urlIndex = bnTrailArray.indexOf(prodPage);
        }

        // Found in viewed list, remove it and put on end.
        if (urlIndex != -1) {
            bnTrailArray.splice(urlIndex, 1);
            bnTrailArray.unshift(prodPage);
            var cookieJSON = bnCommon.arrayToJSON(bnTrailArray);
            BaynoteAPI.setCookie('bn_documentVisitsTrail', cookieJSON, '/', expires, window.location.host);
        }
        // Not found in viewed list but list already 3 items long.
        else if (bnTrailArray.length == 3 && urlIndex == -1) {
            // Remove first item and put new URL on end.
            bnTrailArray.splice(-1, 1);
            bnTrailArray.unshift(prodPage);
            var cookieJSON = bnCommon.arrayToJSON(bnTrailArray);
            BaynoteAPI.setCookie('bn_documentVisitsTrail', cookieJSON, '/', expires, window.location.host);
        } else if (bnTrailArray.length > 3) {
		BaynoteAPI.removeCookie("bn_documentVisitsTrail");
		addToVisitsTrail(prodPage);
	}
        // Not already found in viewed list and list is shorter than 3. Add it on end.
        else {
            bnTrailArray.unshift(prodPage);
            var cookieJSON = bnCommon.arrayToJSON(bnTrailArray);
            BaynoteAPI.setCookie('bn_documentVisitsTrail', cookieJSON, '/', expires, window.location.host);
        }

    }
    // bnTrail does not exist
    else {
        var bnTrailArray = new Array();
        bnTrailArray.push(prodPage);
        var cookieJSON = bnCommon.arrayToJSON(bnTrailArray);
        BaynoteAPI.setCookie('bn_documentVisitsTrail', cookieJSON, '/', expires, window.location.host);
    }
}

function bn_normalize(url) {
    var regex = new RegExp("^https?://.*farnell.com/.*/dp/([^?]*).*");
    var domain = "farnell";
    if (location.hostname.indexOf("newark")> -1) {domain = "newark"; regex= new RegExp("^https?://.*newark.com/.*/dp/([^?]*).*");}
    if (location.hostname.indexOf("element14") > -1) {domain = "element14";regex= new RegExp("^https?://.*element14.com/.*/dp/([^?]*).*");}
    var match = regex.exec(url);
    var host = location.hostname.split(domain)[0];
    if (match && match.length > 1) {
        return "http://"+host+domain+".com/jsp/search/productdetail.jsp?SKU=" + match[1];
    }
    return null;
}

function bn_getUrls(parent) {
    var array = new Array();
    var right = document.getElementById(parent);
    if (right) {
        var divs = right.getElementsByTagName("div");
        for (var i = 0; i < divs.length; i++) {
            if (divs[i].className && divs[i].className == "merchImage") {
                var a = divs[i].getElementsByTagName("a");
                if (a[0] && a[0].href) {
                    var url = bn_normalize(a[0].href);
            if (url) {
                array.push(url);
            }
                }
            }
        }
    }
    return array;
}

//needed ?
function bn_setContext(tag) {
    var urls = new Array();
    urls.push(bn_normalize(location.href));
    urls = urls.concat(bn_getUrls("L5 Right 3"));
    urls = urls.concat(bn_getUrls("L5 Right 2"));
    urls = urls.concat(bn_getUrls("A"));
    urls = urls.concat(bn_getUrls("Bottom Category"));
    urls = urls.concat(bn_getUrls("Right 3 Category"));
    var attrFilter = tag.attrFilter || "";
    for (var i = 0; i < urls.length; i++) {
        if (attrFilter) {
            attrFilter = attrFilter + ",";
        }
        attrFilter = attrFilter + '-bnUri:"' + urls[i] + '"';
    }
    if (attrFilter) {
        tag.attrFilter = attrFilter;
    }
}

function bn_normalizeParentCat(parentCat) {
    parentCat = parentCat.trim().toLowerCase();
    parentCat = parentCat.replace(/ /g, "-");
    parentCat = parentCat.replace(/&amp;/g, "-");
    parentCat = parentCat.replace(/&/g, "-");
    parentCat = parentCat.replace(/,/g, "-");
    parentCat = parentCat.replace(/-+/g, "-");
    if (parentCat) {
        return parentCat;
    }
    return "";
}

function bn_setCategoryFilter(tag) {
    //console.log('tag',tag);
    if (document.getElementById("bn_browse_2")) {
        if (!document.getElementById("breadcrumb") || !document.getElementById("breadcrumb").getElementsByTagName("li")[1]){return "";}
        var parentCat = document.getElementById("breadcrumb").getElementsByTagName("li")[1].getElementsByTagName("a")[0].innerHTML;
        if (parentCat) {
            if (tag) {
                    tag.attrFilter = tag.attrFilter ? tag.attrFilter + "," : "";
                    tag.attrFilter = tag.attrFilter + "ParentCategoryURL:" + bn_normalizeParentCat(parentCat);
            } else {
                return parentCat;
            }
        }
    }
    return false;
}

// TODO: Normalise for Oscilloscopes in US
function mer_getCategoryFilter() {
    if (pf.param.pageMode == "search") {
        return false;
    }
    if ( !document.getElementById("breadcrumb") ) {
        return false;
    }
    var ele_li = document.getElementById("breadcrumb").getElementsByTagName("li");
    var ele_li_len = ele_li.length;

    if ( ele_li_len < 2 ) {
        return false;
    }

    var catFilter = [];

    if ( ele_li_len > 2 ) {
        //TODO test a[0]
        catFilter.push(ele_li[1].getElementsByTagName("a")[0].innerHTML); // SubCategory
        catFilter.push(ele_li[2].getElementsByTagName("a")[0].innerHTML); // ParentCategory
    } else if ( ele_li_len == 2 ) {
        catFilter.push(ele_li[1].getElementsByTagName("a")[0].innerHTML); // ParentCategory
    }
    return catFilter;
}

function bn_isProduct() {
    var product = new RegExp("^https?://www\.newark\.com/.*/dp/[^\?]+.*");
    if (location.hostname.indexOf("newark")> -1) {product= new RegExp("^https?://.*newark.com/.*/dp/([^?]*).*");}
    if (location.hostname.indexOf("element14") > -1) {product= new RegExp("^https?://.*element14.com/.*/dp/([^?]*).*");}
    if (location.hostname.indexOf("farnell") > -1) {product= new RegExp("^https?://.*farnell.com/.*/dp/([^?]*).*");}    
    return product.test(location.href);
}

/*
* function to add additional parameters to rec call, example deduplicate=true.
*/
function bn_setEngine(tag) {
    if ((document.getElementById("bn_level5_3") && document.getElementById("bn_level5_2")) || (document.getElementById("bn_browse_1") && document.getElementById("bn_browse_2"))) {
        tag.extraParams = tag.extraParams || {};
        tag.extraParams.deduplicate = true;
    }
}

//notused
function bn_setCartContext(tag) {
    var bn_CartProducts = [];
    var orderDetails = document.getElementById("order_details");
    if(!orderDetails) return;
    var a = orderDetails.getElementsByTagName("a");
    var regex = new RegExp("^(https?://(?:www\.)?de\.(?:(?:uatprdeu|psstgeu)\.)?farnell\.com)/.*/dp/([^?]*).*");
    for (var i = 0; i < a.length; i++) {
        if (a[i].className == "bnSku" && a[i].href) {
            var match = regex.exec(a[i].href);
            if (match && match[1]) {
                bn_CartProducts.push(match[1] + "/jsp/search/productdetail.jsp?SKU=" + match[2]);
            }
        }
    }
    if(typeof(bn_CartProducts) != "undefined" && BaynoteAPI.isNotEmpty(bn_CartProducts)){
        if (tag) {
            tag.listUrls = bn_CartProducts;
        } else {
            return bn_CartProducts;
        }
    }
}

function iclink1(seoURL, SKU, icsuffix) {
    var suf = "MER=BN-";
    var prf = "?";
    var prot = "http://";
    var host = window.location.hostname;
    if (!icsuffix) icsuffix = "";
    if (seoURL.indexOf("?")> -1 ) prf="&";
    if (seoURL.indexOf("http://") > -1 ) {prot = ""; host=""}
    return prot+host+seoURL + prf+suf+ icsuffix + SKU;
}

/** 'mer' link **/
function mer_iclink1(seoURL, slot, icsuffix) {
    var suf = "MER=";
    var prf = "?";
    var prot = "http://";
    var host = window.location.hostname;
    if (!icsuffix) icsuffix = "";
    if (seoURL.indexOf("?")> -1 ) prf="&";
    if (seoURL.indexOf("http://") > -1 ) {prot = ""; host="";}
    return prot+host+seoURL + prf+suf+ icsuffix + slot;
}

/** 'mer' set placeholder attribute **/
function mer_setAttribute(aPlaceholderNames,sAttrName,sAttrValue) {
    var aLength = aPlaceholderNames.length;
    if (aLength > 0) {
        for (var i = 0; i < aLength; i++) {
            //console.log("aPlaceholderNames[i]",i,aPlaceholderNames[i]);
            var ele = document.getElementById(aPlaceholderNames[i]);
            if ( ele !== null ) {
                ele.setAttribute(sAttrName, sAttrValue);
            }
        }
    }
}

/** 'mer' check pf.minibasket.addEvents(); **/
function mer_hasPFMiniBasketObj() {
    if ( typeof pf !== "undefined" || pf !== null ) {
        if ( typeof pf.minibasket !== "undefined" || pf.minibasket !== null ) {
            if ( typeof pf.minibasket.addEvents !== "undefined" || pf.minibasket.addEvents !== null ) {
                return true;
            }
        }
    }
    return false;
}

/** 'mer' is devEnv @returns {boolean} **/
function mer_isDevEnv() {
    return /ftprd|uatprd/.test(window.location.hostname);
}

/** 'mer' is 'newark' @returns {boolean} **/
function mer_isNewark() {
    return /newark/.test(window.location.hostname);
}

/** 'mer' is 'element14' @returns {boolean} **/
function mer_isElement14() {
    return /element14/.test(window.location.hostname);
}


function thumbnail(ThumbUrl,height) {
    var thumb = ThumbUrl.split(",")[0];
    var wdth = "";
    if (height) { wdth = ' style="height:'+height+'" ';}
    if (thumb.indexOf("<img") != -1) {
        return  ThumbUrl;
    } else {
        if (thumb.indexOf("http:") != -1) {
            return '<img src="' + thumb.split("http:")[1] + '"/>';
        } else {
            return 	'<img src="' + thumb + '" '+wdth+'/>';
        }
    }
}

/** Start - 'mer' Lang elements, Price, UOM, Add to Cart */
var bnUOM = {};
/* UOM Global */
bnUOM.KIT = {"es-mx":"Kit","en-ca":"Kit ","en-us":"Kit ","en-au":"Kit ","en-in":"Kit ","en-my":"Kit ","en-nz":"Kit ","en-ph":"Kit ","en-sg":"Kit ","ko-kr":"Ã­â€šÂ¤Ã­Å Â¸","th-th":"Ã Â¸Å Ã Â¸Â¸Ã Â¸â€","zh-cn":"Ã¦Â¯ÂÃ¥Â¥â€”","bg-bg":"ÃÅ¡ÃÂ¾ÃÂ¼ÃÂ¿ÃÂ»ÃÂµÃÂºÃ‘â€š","cs-cz":"Sada","de-at":"Kit","de-ch":"Kit","de-de":"Kit","da-dk":"Kit","en-ex":"Kit ","en-ie":"Kit ","en-il":"Kit ","en-gb":"Kit ","es-es":"Kit","et-ee":"Komplekt","fi-fi":"sarja","fr-be":"Kit","fr-ch":"Kit","fr-fr":"Kit","hu-hu":"K\u00E9szlet","it-it":"Kit","lt-lt":"rinkinys","lv-lv":"komplekts","nl-be":"KIT Ãƒ ","nl-nl":"KIT Ãƒ ","no-no":"Sett","onecall":"Kit ","pl-pl":"zestaw","pt-pt":"Kit","ro-ro":"Kit","ru-ru":"ÃÅ¡ÃÂ¾ÃÂ¼ÃÂ¿ÃÂ»ÃÂµÃÂºÃ‘â€š","sv-se":"Kit","sk-sk":"sÃƒÂºprava","sl-si":"komplet","tr-tr":"Kit"};
bnUOM.EACH = {"es-mx":"Cada","en-ca":"Each","en-us":"Each","en-au":"Each","en-in":"Each","en-my":"Each","en-nz":"Each","en-ph":"Each","en-sg":"Each","ko-kr":"ÃªÂ°Â","th-th":"Ã Â¸Â­Ã Â¸Â±Ã Â¸â„¢Ã Â¸Â¥Ã Â¸Â°","zh-cn":"Ã¦Â¯ÂÃ¤Â¸Âª","bg-bg":"Ãâ€”ÃÂ° ÃÂ±Ã‘â‚¬ÃÂ¾ÃÂ¹ÃÂºÃÂ°","cs-cz":"Jednotliv\u011B","de-at":"StÃƒÂ¼ck","de-ch":"Einzeln","de-de":"StÃƒÂ¼ck","da-dk":"Styk","en-ex":"Each","en-ie":"Each","en-il":"Each","en-gb":"Each","es-es":"Cada","et-ee":"TÃƒÂ¼kitoode","fi-fi":"kappale","fr-be":"PiÃƒÂ¨ce","fr-ch":"Unit\u00E9","fr-fr":"PiÃƒÂ¨ce","hu-hu":"darab","it-it":"UnitÃƒ ","lt-lt":"Vnt.","lv-lv":"VienÃ„Â«ba","nl-be":"per stuk","nl-nl":"per stuk","no-no":"Per stykk","onecall":"Each","pl-pl":"sztuka","pt-pt":"Cada","ro-ro":"La bucat\u0103","ru-ru":"ÃÂ¨Ã‘â€šÃ‘Æ’ÃÂºÃÂ°","sv-se":"Styck","sk-sk":"kus","sl-si":"kos","tr-tr":"Adet"};
bnUOM.POUND = {"es-mx":"Libra","en-ca":"Pound","en-us":"Pound","en-au":"Pound","en-in":"Pound","en-my":"Pound","en-nz":"Pound","en-ph":"Pound","en-sg":"Pound","ko-kr":"Ã­Å’Å’Ã¬Å¡Â´Ã«â€œÅ“","th-th":"Ã Â¸â€ºÃ Â¸Â­Ã Â¸â„¢Ã Â¸â€Ã Â¹Å’","zh-cn":"Ã§Â£â€¦","bg-bg":"ÃÂ¤Ã‘Æ’ÃÂ½Ã‘â€š","cs-cz":"Libra","de-at":"Pfund","de-ch":"Kilo","de-de":"Pfund","da-dk":"Pund","en-ex":"Pound","en-ie":"Pound","en-il":"Pound","en-gb":"Pound","es-es":"Libra","et-ee":"Nael","fi-fi":"pauna","fr-be":"Livre","fr-ch":"Kilogramme","fr-fr":"Livre","hu-hu":"Kilogramm","it-it":"Libbra","lt-lt":"svaras","lv-lv":"mÃ„ÂrciÃ…â€ a","nl-be":"pond","nl-nl":"pond","no-no":"Pund","onecall":"Pound","pl-pl":"funt","pt-pt":"libra","ro-ro":"Kilogram","ru-ru":"ÃÂ¤Ã‘Æ’ÃÂ½Ã‘â€š","sv-se":"Pund","sk-sk":"libra","sl-si":"funt","tr-tr":"Libre"};
bnUOM.PACK = {"es-mx":"Paquete de","en-ca":"Pack of","en-us":"Pack of","en-au":"Pack of","en-in":"Pack of","en-my":"Pack of","en-nz":"Pack of","en-ph":"Pack of","en-sg":"Pack of","ko-kr":"Ã­Å’Â©","th-th":"Ã Â¹ÂÃ Â¸Å¾Ã Â¹â€¡Ã Â¸â€žÃ Â¸Â¥Ã Â¸Â°","zh-cn":"Ã¦Â¯ÂÃ§Â»â€ž","bg-bg":"ÃÅ¾ÃÂ¿ÃÂ°ÃÂºÃÂ¾ÃÂ²ÃÂºÃÂ° ÃÂ¾Ã‘â€š","cs-cz":"Balen\u00ED","de-at":"Packung Ãƒ ","de-ch":"Packung","de-de":"Packung Ãƒ ","da-dk":"Pakke med","en-ex":"Pack of","en-ie":"Pack of","en-il":"Pack of","en-gb":"Pack of","es-es":"Paquete de","et-ee":"Pakendis","fi-fi":"pakkaus jossa","fr-be":"Paquet de","fr-ch":"Paquet de","fr-fr":"Paquet de","hu-hu":"Csomag","it-it":"Confezione da","lt-lt":"PakuoÃ„ÂiÃ…Â³:","lv-lv":"Iepakojums:","nl-be":"PAKKET Ãƒ ","nl-nl":"PAKKET Ãƒ ","no-no":"Pakke med","onecall":"Pack of","pl-pl":"paczka, sztuk","pt-pt":"Pacote de","ro-ro":"Pachet de","ru-ru":"ÃÅ¸ÃÂ°ÃÂºÃÂµÃ‘â€š ÃÂ¸ÃÂ·","sv-se":"Pack","sk-sk":"PoÃ„Âet kusov v balenÃƒÂ­","sl-si":"paket po","tr-tr":"Paket"};
bnUOM.PAIR = {"es-mx":"Par","en-ca":"Pair","en-us":"Pair","en-au":"Pair","en-in":"Pair","en-my":"Pair","en-nz":"Pair","en-ph":"Pair","en-sg":"Pair","ko-kr":"Ã¬Å’Â","th-th":"Ã Â¸â€žÃ Â¸Â¹Ã Â¹Ë†","zh-cn":"Ã¤Â¸â‚¬Ã¥Â¯Â¹","bg-bg":"ÃÂ§ÃÂ¸Ã‘â€žÃ‘â€š","cs-cz":"P\u00E1r","de-at":"Paar","de-ch":"Paar","de-de":"Paar","da-dk":"Par med","en-ex":"Pair","en-ie":"Pair","en-il":"Pair","en-gb":"Pair","es-es":"Par","et-ee":"Paar","fi-fi":"pari,","fr-be":"Paire","fr-ch":"Paire","fr-fr":"Paire","hu-hu":"P\u00E1r","it-it":"Coppia","lt-lt":"PorÃ…Â³:","lv-lv":"PÃ„Âris:","nl-be":"paar","nl-nl":"paar","no-no":"Par","onecall":"Pair","pl-pl":"para","pt-pt":"par","ro-ro":"Pereche","ru-ru":"ÃÅ¸ÃÂ°Ã‘â‚¬ÃÂ°","sv-se":"Par","sk-sk":"pÃƒÂ¡r","sl-si":"par","tr-tr":"Ãƒâ€¡ift"};
bnUOM.REEL = {"es-mx":"Rollo de","en-ca":"Reel of","en-us":"Reel of","en-au":"Reel of","en-in":"Reel of","en-my":"Reel of","en-nz":"Reel of","en-ph":"Reel of","en-sg":"Reel of","ko-kr":"Ã«Â¦Â´","th-th":"Ã Â¸Â¡Ã Â¹â€°Ã Â¸Â§Ã Â¸â„¢Ã Â¸Â¥Ã Â¸Â°","zh-cn":"Ã¥ÂÂ·","bg-bg":"Ã ÃÂ¾ÃÂ»ÃÂºÃÂ° ÃÂ¾Ã‘â€š","cs-cz":" Kotouc","de-at":"Rolle Ãƒ ","de-ch":"Rolle","de-de":"Rolle Ãƒ ","da-dk":"Rulle med","en-ex":"Reel of","en-ie":"Reel of","en-il":"Reel of","en-gb":"Reel of","es-es":"Rollo de","et-ee":"Poolis","fi-fi":"kela jossa","fr-be":"Bobine de","fr-ch":"Bobine de","fr-fr":"Bobine de","hu-hu":"ennyi darab tekercs:","it-it":"Reel da","lt-lt":"RulonÃ…Â³:","lv-lv":"Tinums:","nl-be":"REEL Ãƒ ","nl-nl":"REEL Ãƒ ","no-no":"Reel med","onecall":"Reel of","pl-pl":"szpula","pt-pt":"Rolo de","ro-ro":"Bobina","ru-ru":"ÃÅ¡ÃÂ°Ã‘â€šÃ‘Æ’Ã‘Ë†ÃÂºÃ‘Æ’ ÃÂ¸ÃÂ·","sv-se":"Rulle","sk-sk":"PoÃ„Âet kusov na cievke","sl-si":"kolut po","tr-tr":"Makara"};
bnUOM.SPOOL= {"es-mx":"Carrete de","en-ca":"Spool of","en-us":"Spool of","en-au":"Spool of","en-in":"Spool of","en-my":"Spool of","en-nz":"Spool of","en-ph":"Spool of","en-sg":"Spool of","ko-kr":"Ã¬Å Â¤Ã­â€™â‚¬","th-th":"Ã Â¸Â¡Ã Â¹â€°Ã Â¸Â§Ã Â¸â„¢Ã Â¸Â¥Ã Â¸Â°","zh-cn":"Ã¥ÂÂ·","bg-bg":"ÃÅ“ÃÂ°ÃÂºÃÂ°Ã‘â‚¬ÃÂ° ÃÂ¾Ã‘â€š","cs-cz":"C\u00EDvka","de-at":"Rolle Ãƒ ","de-ch":"Spule","de-de":"Rolle Ãƒ ","da-dk":"Spole med","en-ex":"Spool of","en-ie":"Spool of","en-il":"Spool of","en-gb":"Spool of","es-es":"Carrete de","et-ee":"Poolis","fi-fi":"rulla jossa","fr-be":"Enrouleur de","fr-ch":"Enrouleur de","fr-fr":"Enrouleur de","hu-hu":"K\u00E1beldob","it-it":"Bobina da","lt-lt":"RiÃ„ÂiÃ…Â³","lv-lv":"Spole:","nl-be":"SPOEL Ãƒ ","nl-nl":"SPOEL Ãƒ ","no-no":"Spole med","onecall":"Spool of","pl-pl":"szpula","pt-pt":"Bobina de","ro-ro":"Rol\u0103","ru-ru":"ÃÅ¡ÃÂ°Ã‘â€šÃ‘Æ’Ã‘Ë†ÃÂºÃ‘Æ’ ÃÂ¸ÃÂ·","sv-se":"Spole","sk-sk":"PoÃ„Âet kusov na cievke","sl-si":"zvitek po","tr-tr":"SargÃ„Â±"};
bnUOM.METER = {"es-mx":"Metro","en-ca":"Meter","en-us":"Meter","en-au":"Metre","en-in":"Metre","en-my":"Metre","en-nz":"Metre","en-ph":"Metre","en-sg":"Metre","ko-kr":"Ã«Â¯Â¸Ã­â€žÂ°","th-th":"Ã Â¹â‚¬Ã Â¸Â¡Ã Â¸â€¢Ã Â¸Â£","zh-cn":"Ã§Â±Â³","bg-bg":"ÃÅ“ÃÂµÃ‘â€šÃ‘Å Ã‘â‚¬","cs-cz":"Metr","de-at":"Meter","de-ch":"Meter","de-de":"Meter","da-dk":"Meter","en-ex":"Metre","en-ie":"Metre","en-il":"Metre","en-gb":"Metre","es-es":"Metro","et-ee":"Meeter","fi-fi":"metri","fr-be":"MÃƒÂ¨tre","fr-ch":"M\u00E8tre","fr-fr":"MÃƒÂ¨tre","hu-hu":"M\u00E9ter","it-it":"Metro","lt-lt":"metras","lv-lv":"metrs","nl-be":"meter","nl-nl":"meter","no-no":"Meter","onecall":"Metre","pl-pl":"metr","pt-pt":"metro","ro-ro":"Metru","ru-ru":"ÃÅ“ÃÂµÃ‘â€šÃ‘â‚¬","sv-se":"Meter","sk-sk":"meter","sl-si":"meter","tr-tr":"Metre"};
bnUOM.SET = {"es-mx":"Juego","en-ca":"Set ","en-us":"Set ","en-au":"Set ","en-in":"Set ","en-my":"Set ","en-nz":"Set ","en-ph":"Set ","en-sg":"Set ","ko-kr":"Ã¬â€žÂ¸Ã­Å Â¸","th-th":"Ã Â¹â‚¬Ã Â¸â€¹Ã Â¹â€¡Ã Â¸â€¢","zh-cn":"Ã¦Â¯ÂÃ¥Â¥â€”","bg-bg":"ÃÂÃÂ°ÃÂ±ÃÂ¾Ã‘â‚¬","cs-cz":"Sada","de-at":"Satz","de-ch":"Satz","de-de":"Satz","da-dk":"SÃƒÂ¦t","en-ex":"Set ","en-ie":"Set ","en-il":"Set ","en-gb":"Set ","es-es":"Juego","et-ee":"Komplekt","fi-fi":"lajitelma","fr-be":"Assortiment","fr-ch":"Assortiment","fr-fr":"Assortiment","hu-hu":" K\u00E9szlet","it-it":"Set","lt-lt":"komplektas","lv-lv":"komplekts","nl-be":"SET Ãƒ ","nl-nl":"SET Ãƒ ","no-no":"Sett","onecall":"Set ","pl-pl":"zestaw","pt-pt":"Conjunto","ro-ro":"Set ","ru-ru":"ÃÂÃÂ°ÃÂ±ÃÂ¾Ã‘â‚¬","sv-se":"Set","sk-sk":"sÃƒÂºprava","sl-si":"garnitura","tr-tr":"Set"};
bnUOM.KG = {"es-mx":"KG","en-ca":"KG","en-us":"KG","en-au":"KG","en-in":"KG","en-my":"KG","en-nz":"KG","en-ph":"KG","en-sg":"KG","ko-kr":"Ã­â€šÂ¬Ã«Â¡Å“ÃªÂ·Â¸Ã«Å¾Â¨","th-th":"Ã Â¸ÂÃ Â¸Â´Ã Â¹â€šÃ Â¸Â¥Ã Â¸ÂÃ Â¸Â£Ã Â¸Â±Ã Â¸Â¡","zh-cn":"Ã¥ÂÆ’Ã¥â€¦â€¹","bg-bg":"ÃÅ¡ÃÂ¸ÃÂ»ÃÂ¾ÃÂ³Ã‘â‚¬ÃÂ°ÃÂ¼","cs-cz":"Libra","de-at":"KG","de-ch":"Kilo","de-de":"KG","da-dk":"KG","en-ex":"KG","en-ie":"KG","en-il":"KG","en-gb":"KG","es-es":"KG","et-ee":"Kilo","fi-fi":"kilogramma","fr-be":"KG","fr-ch":"Kilogramme","fr-fr":"KG","hu-hu":"Kilogramm","it-it":"KG","lt-lt":"Kilogramas","lv-lv":"kilograms","nl-be":"kg","nl-nl":"kg","no-no":"Pund","onecall":"KG","pl-pl":"KG","pt-pt":"kg","ro-ro":"Kilogram","ru-ru":"ÃÅ¡ÃÂ¸ÃÂ»ÃÂ¾ÃÂ³Ã‘â‚¬ÃÂ°ÃÂ¼ÃÂ¼","sv-se":"KG","sk-sk":"KG","sl-si":"KG","tr-tr":"KG"};

//bnUOM.KIT = {"es-mx":"","en-ca":"","en-au":"","en-sg":"","en-us":"","en-gb":"","de-de":"StÃƒÂ¼ck: 1"};
//bnUOM.EACH = {"es-mx":"","en-ca":"","en-au":"","en-sg":"","en-us":"","en-gb":"","de-de":"StÃƒÂ¼ck: 1"};
//bnUOM.POUND	={"es-mx":"POUND","en-ca":"POUND","en-au":"POUND","en-sg":"POUND","en-us":"POUND","en-gb":"POUND","de-de":"Pfund"};
//bnUOM.PACK	={"es-mx":"PACK","en-ca":"PACK","en-au":"PACK","en-sg":"PACK","en-us":"PACK","en-gb":"PACK","de-de":"Packung Ã¯Â¿Â½ "};
//bnUOM.PAIR	={"es-mx":"PAIR","en-ca":"PAIR","en-au":"PAIR","en-sg":"PAIR","en-us":"PAIR","en-gb":"PAIR","de-de":"Paar"};
//bnUOM.REEL	={"es-mx":"REEL","en-ca":"REEL","en-au":"REEL","en-sg":"REEL","en-us":"REEL","en-gb":"REEL","de-de":"Rolle Ã¯Â¿Â½ "};
//bnUOM.SPOOL	={"es-mx":"SPOOL","en-ca":"SPOOL","en-au":"SPOOL","en-sg":"SPOOL","en-us":"SPOOL","en-gb":"SPOOL","de-de":"Rolle Ã¯Â¿Â½ "};
//bnUOM.METER	={"es-mx":"METER","en-ca":"METER","en-au":"METER","en-sg":"METER","en-us":"METER","en-gb":"METER","de-de":"Meter"};
//bnUOM.SET	={"es-mx":"SET","en-ca":"SET","en-au":"SET","en-sg":"SET","en-us":"SET","en-gb":"SET","de-de":"Satz Ã¯Â¿Â½ "};
//bnUOM.KG	={"es-mx":"KG","en-ca":"KG","en-au":"KG","en-sg":"KG","en-us":"KG","en-gb":"KG","de-de":"KG"};

var TAPE_REEL_CUT= "TAPE & REEL CUT";
var TAPE_REEL_FULL= "TAPE & REEL FULL";

bnUOM.TAPE_REEL_CUT = {"es-mx":"Cinta ","en-ca":"Cut Tape","en-us":"Cut Tape","en-au":"Cut Tape","en-in":"Cut Tape","en-my":"Cut Tape","en-nz":"Cut Tape","en-ph":"Cut Tape","en-sg":"Cut Tape","ko-kr":"Ã¬Â»Â· Ã­â€¦Å’Ã¬ÂÂ´Ã­â€â€ž","th-th":"Ã Â¹â‚¬Ã Â¸â€”Ã Â¸â€ºÃ Â¸â€¢Ã Â¸Â±Ã Â¸â€","zh-cn":"Ã¥Ë†â€¡Ã¥â€°Â²Ã¥ÂÂ·Ã¥Â¸Â¦","bg-bg":"ÃÅ¸Ã‘â‚¬ÃÂµÃÂºÃ‘Å Ã‘ÂÃÂ½ÃÂ°Ã‘â€šÃÂ° ÃÂ»ÃÂµÃÂ½Ã‘â€šÃÂ°","cs-cz":"St\u0159\u00EDhan\u00E1 p\u00E1ska","de-at":"Gurtabschnitte","de-ch":"Gurtabschnitte","de-de":"Gurtabschnitte","da-dk":"Klippet bÃƒÂ¥nd","en-ex":"Cut Tape","en-ie":"Cut Tape","en-il":"Cut Tape","en-gb":"Cut Tape","es-es":"Cinta ","et-ee":"LÃƒÂµigatud linditÃƒÂ¼kk","fi-fi":"YksittÃƒÂ¤ispakkaus","fr-be":"Bandes dÃƒÂ©coupÃƒÂ©es","fr-ch":"Bandes d\u00E9coup\u00E9es","fr-fr":"Bandes dÃƒÂ©coupÃƒÂ©es","hu-hu":"Szalagos","it-it":"Nastro pretagliato","lt-lt":"Nupjauta juosta","lv-lv":"GrieÃ…Â¾amÃ„Â lente","nl-be":"Afgesneden tape","nl-nl":"Afgesneden tape","no-no":"Cut Tape","onecall":"Cut Tape","pl-pl":"TaÃ…â€ºma ciÃ„â„¢ta","pt-pt":"Segmento de fita","ro-ro":"Band\u0103 t\u0103iat\u0103","ru-ru":"Ã ÃÂ°ÃÂ·Ã‘â‚¬ÃÂµÃÂ·ÃÂ½ÃÂ°Ã‘Â ÃÂ»ÃÂµÃÂ½Ã‘â€šÃÂ°","sv-se":"Klippt remsa","sk-sk":"StrihanÃƒÂ¡ pÃƒÂ¡ska","sl-si":"Cut Tape","tr-tr":"Kesik Bant"};
bnUOM.TAPE_REEL_FULL = {"es-mx":"Empaquetado en Rollo Completo","en-ca":"Supplied on Full Reel","en-us":"Supplied on Full Reel","en-au":"Supplied on Full Reel","en-in":"Supplied on Full Reel","en-my":"Supplied on Full Reel","en-nz":"Supplied on Full Reel","en-ph":"Supplied on Full Reel","en-sg":"Supplied on Full Reel","ko-kr":"ÃªÂ°Â","th-th":"Ã Â¹ÂÃ Â¸â€¢Ã Â¹Ë†Ã Â¸Â¥Ã Â¸Â°Ã Â¸Â­Ã Â¸Â±Ã Â¸â„¢ Ã Â¸Ë†Ã Â¸Â³Ã Â¸Â«Ã Â¸â„¢Ã Â¹Ë†Ã Â¸Â²Ã Â¸Â¢Ã Â¹â‚¬Ã Â¸â€ºÃ Â¹â€¡Ã Â¸â„¢Ã Â¸Â¡Ã Â¹â€°Ã Â¸Â§Ã Â¸â„¢Ã Â¹â‚¬Ã Â¸â€¢Ã Â¹â€¡Ã Â¸Â¡","zh-cn":"Ã¦Â¯ÂÃ¥ÂÂ·","bg-bg":"Ãâ€”ÃÂ° ÃÂ±Ã‘â‚¬ÃÂ¾ÃÂ¹ÃÂºÃÂ° ÃÂ´ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂµÃÂ½ÃÂ¾ ÃÂ½ÃÂ° Ã‘â€ Ã‘ÂÃÂ»ÃÂ° Ã‘â‚¬ÃÂ¾ÃÂ»ÃÂºÃÂ°","cs-cz":"Cel\u00FD kotou\u010D","de-at":"StÃƒÂ¼ck gegurtet auf Rolle - ganze Rolle","de-ch":"Ganze Rolle","de-de":"StÃƒÂ¼ck gegurtet auf Rolle - ganze Rolle","da-dk":"leveret pÃƒÂ¥ hel rulle","en-ex":"Supplied on Full Reel","en-ie":"Supplied on Full Reel","en-il":"Supplied on Full Reel","en-gb":"Supplied on Full Reel","es-es":"Empaquetado en Rollo Completo","et-ee":"TÃƒÂ¼kitoode saadetakse tÃƒÂ¤ispoolis","fi-fi":"tÃƒÂ¤ysi kela","fr-be":"PiÃƒÂ¨ce fournie en bobine complÃƒÂ¨te","fr-ch":"Bobine compl\u00E8te","fr-fr":"PiÃƒÂ¨ce fournie en bobine complÃƒÂ¨te","hu-hu":"szalag, teljes tekercs","it-it":"UnitÃƒ , fornito in bobina completa","lt-lt":"Vnt. pilna ritÃ„â€”","lv-lv":"VienÃ„Â«ba piegÃ„ÂdÃ„Âta pilnÃ„Â tinumÃ„Â","nl-be":"EN REEL, VOLLEDIG","nl-nl":"EN REEL, VOLLEDIG","no-no":"Full Reel","onecall":"Supplied on Full Reel","pl-pl":"dostarczane na peÃ…â€šnej rolce","pt-pt":"fornecidoa em fita e rolo completos","ro-ro":"Rol\u0103 complet\u0103","ru-ru":"ÃÂ¨Ã‘â€šÃ‘Æ’ÃÂºÃÂ° ÃÅ¸ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂ»Ã‘ÂÃÂµÃ‘â€šÃ‘ÂÃ‘Â ÃÂ½ÃÂ° ÃÂ¿ÃÂ¾ÃÂ»ÃÂ½ÃÂ¾ÃÂ¹ ÃÂºÃÂ°Ã‘â€šÃ‘Æ’Ã‘Ë†ÃÂºÃÂµ","sv-se":"Levereras i hel rulle","sk-sk":"dodÃƒÂ¡vanÃƒÂ© na celej cievke","sl-si":"dobavljen na polnem kolutu","tr-tr":"Tam Makarada SaÃ„Å¸lanÃ„Â±r"};
bnUOM.AMMO = {"es-mx":"Empaquetado en Cinta","en-ca":"Supplied in Ammo Pack","en-us":"Supplied in Ammo Pack","en-au":"Supplied in Ammo Pack","en-in":"Supplied in Ammo Pack","en-my":"Supplied in Ammo Pack","en-nz":"Supplied in Ammo Pack","en-ph":"Supplied in Ammo Pack","en-sg":"Supplied in Ammo Pack","ko-kr":"ÃªÂ°Â","th-th":"Ã Â¹ÂÃ Â¸â€¢Ã Â¹Ë†Ã Â¸Â¥Ã Â¸Â°Ã Â¸Â­Ã Â¸Â±Ã Â¸â„¢ Ã Â¸Ë†Ã Â¸Â³Ã Â¸Â«Ã Â¸â„¢Ã Â¹Ë†Ã Â¸Â²Ã Â¸Â¢Ã Â¹â‚¬Ã Â¸â€ºÃ Â¹â€¡Ã Â¸â„¢Ã Â¸ÂÃ Â¸Â¥Ã Â¹Ë†Ã Â¸Â­Ã Â¸â€¡","zh-cn":"Ã¦Â¯ÂÃ¥Å’â€¦","bg-bg":"Ãâ€”ÃÂ° ÃÂ±Ã‘â‚¬ÃÂ¾ÃÂ¹ÃÂºÃÂ° ÃÂ´ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂµÃÂ½ÃÂ¾ ÃÂ² ÃÂ¾ÃÂ¿ÃÂ°ÃÂºÃÂ¾ÃÂ²ÃÂºÃÂ° Ã‘â€šÃÂ¸ÃÂ¿ Ammo Pack","cs-cz":"P\u00E1s","de-at":"StÃƒÂ¼ck Lieferung in einem Ammo-Pack","de-ch":"Ammo Pack","de-de":"StÃƒÂ¼ck Lieferung in einem Ammo-Pack","da-dk":"leveret i Ammo Pack","en-ex":"Supplied in Ammo Pack","en-ie":"Supplied in Ammo Pack","en-il":"Supplied in Ammo Pack","en-gb":"Supplied in Ammo Pack","es-es":"Empaquetado en Cinta","et-ee":"Ammo Pack","fi-fi":"laatikkopakkaus","fr-be":"PiÃƒÂ¨ce fournie en Ammo Pack","fr-ch":"Ammo Pack","fr-fr":"PiÃƒÂ¨ce fournie en Ammo Pack","hu-hu":"box","it-it":"UnitÃƒ , fornito in Ammo Pack","lt-lt":"Vnt. pakuotÃ„â€” iÃ…Â¡ skyriÃ…Â³","lv-lv":"VienÃ„Â«ba piegÃ„ÂdÃ„Âta virsmas montÃ„ÂÃ…Â¾as komponentu iepakojumÃ„Â","nl-be":"ROL","nl-nl":"ROL","no-no":"stykk leveres i Ammo Pack","onecall":"Supplied in Ammo Pack","pl-pl":"dostarczane w opakowaniu ammo","pt-pt":"fornecidoa num pacote Ammo","ro-ro":"Ambalaj Ammo","ru-ru":"ÃÂ¨Ã‘â€šÃ‘Æ’ÃÂºÃÂ° ÃÅ¸ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂ»Ã‘ÂÃÂµÃ‘â€šÃ‘ÂÃ‘Â ÃÂ² Ã‘â€šÃÂµÃ‘â€¦ÃÂ½ÃÂ¾ÃÂ»ÃÂ¾ÃÂ³ÃÂ¸Ã‘â€¡ÃÂµÃ‘ÂÃÂºÃÂ¾ÃÂ¹ Ã‘Æ’ÃÂ¿ÃÂ°ÃÂºÃÂ¾ÃÂ²ÃÂºÃÂµ","sv-se":"Levereras i Ammo-pack","sk-sk":"dodÃƒÂ¡vanÃƒÂ© v zÃƒÂ¡sobnÃƒÂ­ku","sl-si":"dobavljen kot narezan trak v Ã…Â¡katli","tr-tr":"MÃƒÂ¼himmat Paketinde SaÃ„Å¸lanÃ„Â±r"};
bnUOM.TUBE = {"es-mx":"Empaquetado en Tubo","en-ca":"Supplied in a Tube","en-us":"Supplied in a Tube","en-au":"Supplied in a Tube","en-in":"Supplied in a Tube","en-my":"Supplied in a Tube","en-nz":"Supplied in a Tube","en-ph":"Supplied in a Tube","en-sg":"Supplied in a Tube","ko-kr":"ÃªÂ°Â","th-th":"Ã Â¹ÂÃ Â¸â€¢Ã Â¹Ë†Ã Â¸Â¥Ã Â¸Â°Ã Â¸Â­Ã Â¸Â±Ã Â¸â„¢ Ã Â¸Ë†Ã Â¸Â³Ã Â¸Â«Ã Â¸â„¢Ã Â¹Ë†Ã Â¸Â²Ã Â¸Â¢Ã Â¹â‚¬Ã Â¸â€ºÃ Â¹â€¡Ã Â¸â„¢Ã Â¸Â«Ã Â¸Â¥Ã Â¸Â­Ã Â¸â€","zh-cn":"Ã¦Â¯ÂÃ§Â®Â¡","bg-bg":"Ãâ€”ÃÂ° ÃÂ±Ã‘â‚¬ÃÂ¾ÃÂ¹ÃÂºÃÂ° ÃÂ´ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂµÃÂ½ÃÂ¾ ÃÂ² Ã‘â€šÃ‘â‚¬Ã‘Å ÃÂ±ÃÂ°","cs-cz":"Tuba","de-at":"StÃƒÂ¼ck Lieferung auf Stange","de-ch":"Stange","de-de":"StÃƒÂ¼ck Lieferung auf Stange","da-dk":"leveret i rÃƒÂ¸r","en-ex":"Supplied in a Tube","en-ie":"Supplied in a Tube","en-il":"Supplied in a Tube","en-gb":"Supplied in a Tube","es-es":"Empaquetado en Tubo","et-ee":"TÃƒÂ¼kitoode saadetakse tuubis","fi-fi":"putki","fr-be":"PiÃƒÂ¨ce fournie en tube","fr-ch":"Tube","fr-fr":"PiÃƒÂ¨ce fournie en tube","hu-hu":"tubus","it-it":"UnitÃƒ , fornito in stecca","lt-lt":"Vnt. vamzdis","lv-lv":"VienÃ„Â«ba piegÃ„ÂdÃ„Âta caurulÃ„â€œ","nl-be":"Ãƒ  ","nl-nl":"Ãƒ  ","no-no":"R\u00F8r","onecall":"Supplied in a Tube","pl-pl":"dostarczane w tubie","pt-pt":"fornecidoa num tubo","ro-ro":"Tub","ru-ru":"ÃÂ¨Ã‘â€šÃ‘Æ’ÃÂºÃÂ° ÃÅ¸ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂ»Ã‘ÂÃÂµÃ‘â€šÃ‘ÂÃ‘Â ÃÂ² Ã‘â€šÃ‘â‚¬Ã‘Æ’ÃÂ±ÃÂºÃÂµ","sv-se":"Levereras i rÃƒÂ¶r","sk-sk":"dodÃƒÂ¡vanÃƒÂ© v tube","sl-si":"dobavljen v kanalu","tr-tr":"TÃƒÂ¼pte SaÃ„Å¸lanÃ„Â±r"};

//bnUOM.TAPE_REEL_CUT ={"es-mx":"TAPE & REEL CUT","en-ca":"TAPE & REEL CUT","en-au":"TAPE & REEL CUT","en-sg":"TAPE & REEL CUT","en-us":"TAPE & REEL CUT","en-gb":"TAPE & REEL CUT","de-de":"Gegurtet auf Rolle - Zuschnitt"};
//bnUOM.TAPE_REEL_FULL ={"es-mx":"TAPE & REEL FULL","en-ca":"TAPE & REEL FULL","en-au":"TAPE & REEL FULL","en-sg":"TAPE & REEL FULL","en-us":"TAPE & REEL FULL","en-gb":"TAPE & REEL FULL","de-de":"Gegurtet auf Rolle - ganze Rolle"};
//bnUOM.AMMO	={"es-mx":"AMMO","en-ca":"AMMO","en-au":"AMMO","en-sg":"AMMO","en-us":"AMMO","en-gb":"AMMO","de-de":"AMMO"};
//bnUOM.TUBE	={"es-mx":"TUBE","en-ca":"TUBE","en-au":"TUBE","en-sg":"TUBE","en-us":"TUBE","en-gb":"TUBE","de-de":"Dose Ã¯Â¿Â½ "};

var FULL_WAFFLE_TRAY = "FULL WAFFLE TRAY";
bnUOM.FULL_WAFFLE_TRAY = {"es-mx":"Empaquetado en Bandeja","en-ca":"Supplied in a Waffle Tray","en-us":"Supplied in a Waffle Tray","en-au":"Supplied in a Waffle Tray","en-in":"Supplied in a Waffle Tray","en-my":"Supplied in a Waffle Tray","en-nz":"Supplied in a Waffle Tray","en-ph":"Supplied in a Waffle Tray","en-sg":"Supplied in a Waffle Tray","ko-kr":"ÃªÂ°Â","th-th":"Ã Â¹ÂÃ Â¸â€¢Ã Â¹Ë†Ã Â¸Â¥Ã Â¸Â°Ã Â¸Â­Ã Â¸Â±Ã Â¸â„¢ Ã Â¸Ë†Ã Â¸Â³Ã Â¸Â«Ã Â¸â„¢Ã Â¹Ë†Ã Â¸Â²Ã Â¸Â¢Ã Â¹â‚¬Ã Â¸â€ºÃ Â¹â€¡Ã Â¸â„¢Ã Â¸â€“Ã Â¸Â²Ã Â¸â€","zh-cn":"Ã¦Â¯ÂÃ¥ÂÅ½Ã¥Â¤Â«Ã¦ Â¼Ã§â€ºËœ","bg-bg":"Ãâ€”ÃÂ° ÃÂ±Ã‘â‚¬ÃÂ¾ÃÂ¹ÃÂºÃÂ° ÃÂ´ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂµÃÂ½ÃÂ¾ ÃÂ² ÃÂ³ÃÂ¾Ã‘â€žÃ‘â‚¬ÃÂ¸Ã‘â‚¬ÃÂ°ÃÂ½ÃÂ° ÃÂ¿ÃÂ¾ÃÂ´ÃÂ»ÃÂ¾ÃÂ¶ÃÂºÃÂ°","cs-cz":"Plato","de-at":"StÃƒÂ¼ck Lieferung in einem Waffle-Tray","de-ch":"Supplied in a Waffle Tray","de-de":"StÃƒÂ¼ck Lieferung in einem Waffle-Tray","da-dk":"leveret i Waffle Tray","en-ex":"Supplied in a Waffle Tray","en-ie":"Supplied in a Waffle Tray","en-il":"Supplied in a Waffle Tray","en-gb":"Supplied in a Waffle Tray","es-es":"Empaquetado en Bandeja","et-ee":"Waffle Tray","fi-fi":"lokerikko","fr-be":"PiÃƒÂ¨ce fournie en plateau alvÃƒÂ©olÃƒÂ©","fr-ch":"Plateau alv\u00E9ol\u00E9","fr-fr":"PiÃƒÂ¨ce fournie en plateau alvÃƒÂ©olÃƒÂ©","hu-hu":"V\u00E9dor\u00E1csos t\u00E1lca","it-it":"UnitÃƒ , fornito su vassoio","lt-lt":"Vnt. matricos tipo padÃ„â€”klas","lv-lv":"VienÃ„Â«ba piegÃ„ÂdÃ„Âta vafeÃ„Â¼formas platÃ„â€œ","nl-be":"TRAY Ãƒ  ","nl-nl":"TRAY Ãƒ  ","no-no":"Vaffelbrett","onecall":"Supplied in a Waffle Tray","pl-pl":"dostarczane na tacce","pt-pt":"fornecidoa numa bandeja matricial","ro-ro":"Tava-matrice","ru-ru":"ÃÂ¨Ã‘â€šÃ‘Æ’ÃÂºÃÂ° ÃÅ¸ÃÂ¾Ã‘ÂÃ‘â€šÃÂ°ÃÂ²ÃÂ»Ã‘ÂÃÂµÃ‘â€šÃ‘ÂÃ‘Â ÃÂ½ÃÂ° ÃÂ¼ÃÂ°Ã‘â€šÃ‘â‚¬ÃÂ¸Ã‘â€¡ÃÂ½ÃÂ¾ÃÂ¼ ÃÂ¿ÃÂ¾ÃÂ´ÃÂ´ÃÂ¾ÃÂ½ÃÂµ","sv-se":"Levereras i trÃƒÂ¥g","sk-sk":"dodÃƒÂ¡vanÃƒÂ© vÃ‚ obale s priehradkami","sl-si":"dobavljen na luknjiÃ„Âastem pladnju","tr-tr":"Waffle Tepsisinde SaÃ„Å¸lanÃ„Â±r"};
//bnUOM.FULL_WAFFLE_TRAY	={"es-mx":"FULL WAFFLE TRAY","en-ca":"FULL WAFFLE TRAY","en-au":"FULL WAFFLE TRAY","en-sg":"FULL WAFFLE TRAY","en-us":"FULL WAFFLE TRAY","en-gb":"FULL WAFFLE TRAY","de-de":"Waffle-Tray Ã¯Â¿Â½ "};

var bnPriceText = {"es-mx":"Precio por:","en-ca":"Price For:","en-us":"Price For:","en-au":"Price For:","en-in":"Price For:","en-my":"Price For:","en-nz":"Price For:","en-ph":"Price For:","en-sg":"Price For:","ko-kr":"ÃªÂ°â‚¬ÃªÂ²Â© :","th-th":"Ã Â¸Â£Ã Â¸Â²Ã Â¸â€žÃ Â¸Â²Ã Â¸ÂªÃ Â¸Â³Ã Â¸Â«Ã Â¸Â£Ã Â¸Â±Ã Â¸Å¡:","zh-cn":"Ã¤Â»Â·Ã¦ Â¼Ã¯Â¼Å¡","bg-bg":"ÃÂ¦ÃÂµÃÂ½ÃÂ° ÃÂ·ÃÂ°:","cs-cz":"Cena za:","de-at":"Preiseinheit","de-ch":"Preiseinheit","de-de":"Preiseinheit","da-dk":"Prisen for:","en-ex":"Price For:","en-ie":"Price For:","en-il":"Price For:","en-gb":"Price For:","es-es":"Precio por:","et-ee":"Hinna:","fi-fi":"Pakkauskoko:","fr-be":"Prix pour:","fr-ch":"Prix pour:","fr-fr":"Prix pour:","hu-hu":"CsomagolÃƒÂ¡si egysÃƒÂ©g:","it-it":"Prezzo per:","lt-lt":"Kaina uÃ…Â¾:","lv-lv":"Cena produktam:","nl-be":"Prijs voor:","nl-nl":"Prijs voor:","no-no":"Pris for:","onecall":"Price For:","pl-pl":"Cena dla:","pt-pt":"PreÃƒÂ§o para:","ro-ro":"Pretul pentru:","ru-ru":"ÃÂ¦ÃÂµÃÂ½ÃÂ° ÃÂ´ÃÂ»Ã‘Â:","sv-se":"Pris fÃƒÂ¶r:","sk-sk":"balenie:","sl-si":"Cena za:","tr-tr":"Fiyat:"};
//var bnPriceText = {"en-us":"Price For:","en-gb":"Price For:","de-de":"Preiseinheit"};
var bnLocale = "en-us";

function setLocale() {
    if (!(typeof djConfig == "undefined") && !(djConfig === null)){
            bnLocale = djConfig.locale;
    }
    if (!(typeof dojoConfig == "undefined" ) && !(dojoConfig === null)){
            bnLocale = dojoConfig.locale;
    }
    if (!(typeof dojo == "undefined" ) && !(dojo === null)){
        if (!(typeof dojo.locale == "undefined" ) && !(dojo.locale === null)){
            bnLocale = dojo.locale;
        }
    }

}
setLocale();
function bnTextPrice() {
    var ret =  bnPriceText[bnLocale];
    if (typeof ret == "undefined" ) ret = "Price For:"
    return ret;
}
function bnPrice(price) {

    var curr = {"es-mx":"MXN","en-ca":"CAD","en-us":"USD","en-au":"AUD","en-in":"INR","en-my":"MYR","en-nz":"NZD","en-ph":"PHP","en-sg":"S$","ko-kr":"KRW","th-th":"THB","zh-cn":"CNY","bg-bg":"EUR","cs-cz":"CZK","de-at":"EUR","de-ch":"CHF","de-de":"EUR","da-dk":"DKK","en-ex":"GBP","en-ie":"EUR","en-il":"USD","en-gb":"GBP","es-es":"EUR","et-ee":"EUR","fi-fi":"EUR","fr-be":"EUR","fr-ch":"CHF","fr-fr":"EUR","hu-hu":"HUF","it-it":"EUR","lt-lt":"EUR","lv-lv":"EUR","nl-be":"EUR","nl-nl":"EUR","no-no":"NOK","nb":"NOK","onecall":"GBP","pl-pl":"EUR","pt-pt":"EUR","ro-ro":"RON","ru-ru":"EUR","sv-se":"SEK","sk-sk":"EUR","sl-si":"EUR","tr-tr":"EUR"};
    //var curr = {"it-it":"EUR","en-ca" :"CAD","en-us":"USD","en-gb" : "GBP","en-sg":"S$","zh-cn":"CNY","es-es":"EUR","pt-pt":"EUR","de-de":"EUR"}

    dojo.require("dojo.currency");
    var ret = dojo.currency.format(price, {currency: curr[bnLocale]});
    if (typeof ret == "undefined" ) ret = ""
    if (price == 0 ) ret = "N/A"
    return ret;
}
function uom(UOM) {
    var ret = "1 ";
    if (UOM === TAPE_REEL_CUT) UOM = "TAPE_REEL_CUT";
    if (UOM === TAPE_REEL_FULL) UOM = "TAPE_REEL_FULL";
    if (typeof  bnUOM[UOM] == "undefined" ||  bnUOM[UOM] === null) {/* NO OP */} else {ret = bnUOM[UOM][bnLocale]}

    if (typeof  ret == "undefined" ||  ret === null) {
        ret = "";
    }

    return ret;
}

var bnMinOrder = {"es-mx":"Pedido mÃƒÂ­nimo:","en-ca":"Minimum Order:","en-us":"Minimum Order:","en-au":"Minimum Order:","en-in":"Minimum Order:","en-my":"Minimum Order:","en-nz":"Minimum Order:","en-ph":"Minimum Order:","en-sg":"Minimum Order:","ko-kr":"Ã¬ÂµÅ“Ã¬â€ Å’ Ã¬Â£Â¼Ã«Â¬Â¸:","th-th":"Ã Â¸ÂªÃ Â¸Â±Ã Â¹Ë†Ã Â¸â€¡Ã Â¸â€¹Ã Â¸Â·Ã Â¹â€°Ã Â¸Â­Ã Â¸â€šÃ Â¸Â±Ã Â¹â€°Ã Â¸â„¢Ã Â¸â€¢Ã Â¹Ë†Ã Â¸Â³:","zh-cn":"Ã¦Å“â‚¬Ã¤Â½Å½Ã¨Â®Â¢Ã¨Â´Â­Ã©â€¡Â:","bg-bg":"ÃÅ“ÃÂ¸ÃÂ½ÃÂ¸ÃÂ¼ÃÂ°ÃÂ»ÃÂ½ÃÂ° ÃÂ¿ÃÂ¾Ã‘â‚¬Ã‘Å Ã‘â€¡ÃÂºÃÂ°:","cs-cz":"Minim\u00E1ln\u00ED objednac\u00ED mno\u017Estv\u00ED:","de-at":"MindestauftragsgrÃƒÂ¶ÃƒÅ¸e:","de-ch":" Mindestbestellmenge:","de-de":"MindestauftragsgrÃƒÂ¶ÃƒÅ¸e:","da-dk":"Minimum Order:","en-ex":"Minimum Order:","en-ie":"Minimum Order:","en-il":"Minimum Order:","en-gb":"Minimum Order:","es-es":"Pedido mÃƒÂ­nimo:","et-ee":"Tellimuse Miinimum:","fi-fi":"Minimitilaus:","fr-be":"Commande minimum:","fr-ch":"Commande minimum:","fr-fr":"Commande minimum:","hu-hu":" Minim\u00E1lis rendel\u00E9si mennyis\u00E9g:","it-it":"Ordine minimo:","lt-lt":"MaÃ…Â¾iausias uÃ…Â¾sakymo:","lv-lv":"minimÃ„Âlais pasÃ…Â«tÃ„Â«juma:","nl-be":"Minimum Order:","nl-nl":"Minimum Order:","no-no":"Minimum:","onecall":"Minimum Order:","pl-pl":"Minimalne zamÃƒÂ³wienie:","pt-pt":"Ordem mÃƒÂ­nima:","ro-ro":"Cantitate minim\u0103 comand\u0103:","ru-ru":"ÃÅ“ÃÂ¸ÃÂ½ÃÂ¸ÃÂ¼ÃÂ°ÃÂ»Ã‘Å’ÃÂ½Ã‘â€¹ÃÂ¹ ÃÂ·ÃÂ°ÃÂºÃÂ°ÃÂ·:","sv-se":"Minsta bestÃƒÂ¤llning:","sk-sk":"MinimÃƒÂ¡lna objednÃƒÂ¡vka:","sl-si":"Minimalno naroÃ„Âilo:","tr-tr":"Minimum sipariÃ…Å¸:"};
//var bnMinOrder = {"en-us":"Minimum Order:","en-gb":"Minimum Order:","de-de1":"MindestauftragsgrÃƒÂ¶ÃƒÅ¸e","de-de":""};
function minOrder() {
var ret =  bnMinOrder[bnLocale];
    if (typeof ret == "undefined" ) ret = "Minimum Order:"
    return ret;
}

var buyBN = {"es-mx":"Comprar","en-ca":"Buy","en-us":"Buy","en-au":"Buy","en-in":"Buy","en-my":"Buy","en-nz":"Buy","en-ph":"Buy","en-sg":"Buy","ko-kr":"ÃªÂµÂ¬Ã«Â§Â¤","th-th":"Ã Â¸â€¹Ã Â¸Â·Ã Â¹â€°Ã Â¸Â­","zh-cn":"Ã¨Â´Â­Ã¤Â¹Â°","bg-bg":"ÃÅ¡Ã‘Æ’ÃÂ¿ÃÂµÃ‘â€šÃÂµ","cs-cz":"Koupit","de-at":"Kaufen","de-ch":"Kaufen","de-de":"Kaufen","da-dk":"KÃƒÂ¸b","en-ex":"Buy","en-ie":"Buy","en-il":"Buy","en-gb":"Buy","es-es":"Comprar","et-ee":"Osta","fi-fi":"Osta","fr-be":"Commander","fr-ch":"Commander","fr-fr":"Commander","hu-hu":"Hozz\u00E1ad\u00E1s","it-it":"Acquista","lt-lt":"Ã„Â®sigyti","lv-lv":"Pirkt","nl-be":"Kopen","nl-nl":"Kopen","no-no":"Legg til","onecall":"Buy","pl-pl":"Kup","pt-pt":"Comprar","ro-ro":"Ad\u0103ugare","ru-ru":"ÃÅ¡Ã‘Æ’ÃÂ¿ÃÂ¸Ã‘â€šÃ‘Å’","sv-se":"KÃƒÂ¶p","sk-sk":"KÃƒÂºpiÃ…Â¥","sl-si":"Nakup","tr-tr":"SatÃ„Â±n al"};
//var buyBN = {"en-us":"Buy","en-gb":"Buy","de-de":"Kaufen"};

function bnBuyText() {
var ret =  buyBN[bnLocale];
    if (typeof ret == "undefined" ) ret = "Buy"
    return ret;
}
function bnAddToCart(evt) {
    if (dojo.query('input', evt.target.parentNode)[0]) {
        qtyElement = dojo.query('input', evt.target.parentNode)[0];
    } else {
        qtyElement = dojo.query('input', evt.target.parentNode.parentNode)[0];
    }
    var qty = qtyElement.value || '1';
    var minQty = dojo.attr(qtyElement, 'data-min') || 1;
    var productId = dojo.attr(evt.target, 'data-rel') || dojo.attr(evt.target.parentNode, 'data-rel');
    dojo.place('<div class="loadingGif"></div>', this);
    if (dojo.hasClass(evt.target, "baynote")) {
        if (pf.base.pageName == 'ShoppingBasket') {
            pf.getAtbUrl(productId, true);
        } else {
            pf.addItem(productId, qty, true);
        }
    }
}
Number.prototype.formatMoneyBN = function(c, d, t){
    var n = this, 
        c = isNaN(c = Math.abs(c)) ? 2 : c, 
        d = d == undefined ? "." : d, 
        t = t == undefined ? "," : t, 
        s = n < 0 ? "-" : "", 
        i = parseInt(n = Math.abs(+n || 0).toFixed(c)) + "", 
        j = (j = i.length) > 3 ? j % 3 : 0,
        re = /^([0-9]{2,}?)0+/,
        r = Math.abs(n - i).toFixed(c).slice(2);
        var test = s + (j ? i.substr(0, j) + t : "");
        var test1 = i.substr(j).replace(/(\d{3})(?=\d)/g, "$1" + t);
        var test2 = re.exec(r) ?(c ? d + re.exec(r)[1] : "") : 0;
    return test + test1 + test2;
};

/** End - 'mer' Lang elements, Price, UOM, Add to Cart */

/***********************************/
/** Start - Render template styles */

/******* START: 'mer' *******/

/**
 * 'mer' product cell presentation
 * @param recData
 * @param icsuffix
 * @param placeholderName
 * @param name
 * @param hasBuyBtn
 * @returns {string}
 */
function mer_html(recData, icsuffix, placeholderName, name , hasBuyBtn) {

    var theProduct = mapRec(recData);

    var hasPrice = false;
    if ( (bnLocale.indexOf("en-gb") > -1) || (bnLocale.indexOf("en-us") > -1) ) {
        hasPrice = theProduct.Price > 0;
    }

    var prodSource = "";
    var classHide = " hide";

    /* Start: New Render */
    prodSource += '<li>' +
        '<div id="bn_g_result' + placeholderName + '_' + theProduct.slot + '" class="product content">' +
        '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="' + mer_iclink1(theProduct.seoURL, theProduct.slot, icsuffix) + '">';

    if (theProduct.ThumbUrl) {
        prodSource += '<div class="product-img">' + thumbnail(theProduct.ThumbUrl.replace("thumbnail", "promo")) + '</div>';
    }

    prodSource += '<h3 class="product-mpn">' + theProduct.MfrPartNumber + '</h3>';
    prodSource += '<h4 class="product-description">' + theProduct.ProductName + '</h4>' + '</a>';
    prodSource += '<h5 class="product-manufacturer">' + theProduct.Manufacturer + '</h5>';

    if (hasPrice) {
        prodSource += '<div class="products-price' + ( (hasPrice) ? '' : classHide ) + '">' +
            '<p class="product-moq">' + minOrder() + '&nbsp;' + '<span>' + theProduct.MinOrderQnty + '</span></p>' +
            '<p class="product-price">' + bnPrice(theProduct.Price) + '&nbsp;<span class="product-uom">' + uom(theProduct.UOM) + '</span>&nbsp;</p>' + '</div>';
    }


    if (hasBuyBtn) {

        prodSource += '<div class="merchButton">' +
            '<span class="hide">' +
            '<input value="' + theProduct.MinOrderQnty + '" name="quantity_' + theProduct.SKU + '" data-min="' + theProduct.MinOrderQnty + '"/>' +
            '</span>' +
            '<a class="baynote btn btnTertiary btnSmall leftLoading ajaxAddToCart trolley omTagEvt"' +
            'data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot +
            '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName +
            '" data-rel="' + theProduct.SKU + '" data-prop="eVar8" data-om="BN-Add-'+ theProduct.SKU + '" ' +
            ' href="javascript:void(0);">' +
            bnBuyText() +
            '</a>' +
            '</div>';
    }

    prodSource += '</div></li>';
    /* End: New Render */

    return prodSource;
}

/**
 * 'mer' re-styled product widget mark-up
 * @param {object} params
 * @param {array} params.widget
 * @param {boolean} params.hasBuyBtn
 * @param {string} params.bnPageId (page name)
 **/
function mer_product_results_html(params) {
    //TODO Test params
    var widget, recData, prodSource, placeholderName, displayName, mer_trackcode, hasBuyBtn, isCarousel;
    var slideActiveClass, isHorizontal, isProductDetail, prodGridClass;

    widget = params.widget;
    recData = widget.recData;
    prodSource = "";
    placeholderName = widget.placeholderName;
    displayName = widget.displayName;

    mer_trackcode = placeholderName + '_' + widget.algo + '_';

    isCarousel = false;
    isHorizontal = !((placeholderName.substring(0,11) == "bn_search_1") || ((placeholderName.substring(0,11) == "bn_level5_3") && (bnLocale == "en-gb"))); /* !Original PDP */
    isProductDetail = (placeholderName.substring(0,9) == "bn_level5");

    // Define layout grid class
    if ((bnLocale.indexOf("en-gb") > -1) && (params.bnPageId == "PT_SearchResults") && (widget.name == "SR_BrowseGuide")) {
        prodGridClass = ""; // One Col Vertical Grid format (disable 2 Col)
        /*prodGridClass = "products-two";*/ // Two col Search Results Grid class
    } else if (isHorizontal) {
        prodGridClass = "products-four"; // Four col Horizontal Grid format
    } else {
        prodGridClass = ""; // One Col Vertical Grid format
    }

    hasBuyBtn = (typeof params.hasBuyBtn == "undefined" || params.hasBuyBtn == null) ? false : params.hasBuyBtn;

    prodSource += '<div class="container-fluid">';

    if (isCarousel) {
        prodSource += '<div id="carousel' + placeholderName + '" class="row product-carousel slide"><div class="product-carousel-inner">';
    } else {
        prodSource += '<div class="row"><ul class="products ' + prodGridClass + ' products-merch">';
    }

    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += mer_html(recData[i], mer_trackcode, placeholderName, widget.name, hasBuyBtn, isCarousel);
    }

    if (isCarousel) {
        prodSource += '</div></div>';
        if (!isProductDetail) { prodSource += '<div class="row margin-bottom"></div>'; }
        prodSource += '</div></div>';
    } else {
        prodSource += '</ul></div>';
        if (!isProductDetail) { prodSource += '<div class="row margin-bottom"></div>'; }
        prodSource += '</div>';
    }

    return prodSource;

}

/**
 * 'mer' re-styled product widget mark-up
 * @param {object} params
 * @param {array} params.widget
 * @param {boolean} params.hasBuyBtn
 * @param {string} params.bnPageId (page name)
 **/
// TODO - remove 'devEnv' for Prod
function mer_widget_html(params) {
    var params = params;
    var placeholderName = params.widget.placeholderName;
    var prodSource = "";

    // Check if PDP page && isDevEnv
    // Wrap Collapsible markup
    if ( bnResultsPageType == "ProductDetail" ) { // (R17) full collapsible widget into Baynote placeholder
        if (params.widget.name) {
            prodSource += '<div class="collapsable full open clearBoth" data-dojo-type="Collapsable" dojotype="Collapsable" id="' + params.widget.name + '" widgetid="' + params.widget.name + '">';
            prodSource += '<div id="bn_guidewelcome' + placeholderName + '" class="heading collapsable-trigger headWrap_BAY title bn_g_welcome" data-dojo-attach-point="trigger">';
            prodSource += '<h2>' + params.widget.displayName + '</h2>';
            prodSource += '</div>';
            prodSource += '<div class="collapsable-content container-substitutes merchandising_zone horizontal provider_BAY baynote refactored" data-baynote-page-template="' + params.widget.name + '">';

            prodSource += '<div class="mkt bn_g_area custBought" id="bn_guidearea' + placeholderName + '">';
            prodSource += mer_product_results_html(params);
            prodSource += '</div>';

            prodSource += '</div></div>';
        }

    } else { // Non-R17 widget mark-up
        prodSource += '<div class="mkt bn_g_area custBought" id="bn_guidearea' + placeholderName + '">' +
            '<div id="bn_guidewelcome' + placeholderName + '" class="title bn_g_welcome"><h2>' + params.widget.displayName + '</h2></div>';
        prodSource += mer_product_results_html(params);
        prodSource += '</div>';
    }

    if (document.getElementById(placeholderName)) {
        document.getElementById(placeholderName).innerHTML = prodSource;
    }

}
/******* END: 'mer' mark-up *******/


/******* START: 'legacy' *******/
function vertical_html(recData, icsuffix, placeholderName, name) {
    var theProduct = mapRec(recData);
    var noPrice = theProduct.Price > 0 ? false : true;
    var hideBlockTop ="";
    var hideBlockBot = "";
    var hideBlockTop1 ="";
    var hideBlockBot1 = "";  
    
    if (noPrice) {
        hideBlockTop = '<div class="mk-tab-pane">';
        hideBlockBot = '</div>';
        if (bnLocale.indexOf("en-sg") > -1 || bnLocale.indexOf("en-au") > -1 )
        {
        }        
        else
        {
            hideBlockTop1 = hideBlockTop;
            hideBlockBot1 = hideBlockBot;
        }
    }
    var prodSource = "";
    prodSource += '<div class="item separator" style="float:left">' +
                '<div class="com3001">' +
                    '<div class="head"></div>' +
                    '<div id="bn_g_result' + placeholderName + '_' + theProduct.slot + '" class="content">';
    if (theProduct.ThumbUrl) {
        prodSource += 		'<div class="image">' +
                            '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="' + iclink1(theProduct.seoURL, theProduct.SKU, icsuffix) + '">' +
                                thumbnail(theProduct.ThumbUrl) +
                            '</a>' +                     
                        '</div>';
    }
            
    prodSource += 			'<div class="part_detail with_image">' +
                            '<div class="part_info">' +
                                '<a data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="' + iclink1(theProduct.seoURL, theProduct.SKU, icsuffix) + '">' +
                                    '<div class="mfr_info">' +
                                        '<b>' + theProduct.Manufacturer + '</b>' +
                                        '<br />' +
                                        theProduct.MfrPartNumber +
                                    '</div>' +	                            
                                '</a>' +   
                                '<div class="description">' +
                                    theProduct.ProductName +
                                    '<span class="mk-tab-pane">' +
                                        '<a data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="SKU=' + theProduct.SKU + '"></a>' +
                                    '</span>' +							
                                '</div>' +
                                hideBlockTop +
                                '<div class="merchPrice1">' +
                                    '<p class="priceFor"  style="margin-bottom:2px;float:left">' +
                                        '<strong>'+bnTextPrice()+'&nbsp;</strong>' + uom(theProduct.UOM) + '&nbsp;</p>' +
                                    '<p class="price"  style="margin-bottom:2px;float:left">' +
                                        minOrder()+'&nbsp;' + theProduct.MinOrderQnty + '+&nbsp;' +
                                        '<strong>' + bnPrice(theProduct.Price) + '</strong>' +
                                    '</p>' +
                                '</div>' +
                                hideBlockBot +
                            '</div>' +
                            hideBlockTop1 +
                            '<div class="buy_button, buyLine mk-tab-pane"></div>' +
                                '<div class="mkt_buy_button, mkt_buyLine">' +
                                    '<div class="merchButton" style="float:right">' +
                                    '<span class="mk-tab-pane">' +
                                        '<input value="' + theProduct.MinOrderQnty + '" name="quantity_' + theProduct.SKU + '" data-min="' + theProduct.MinOrderQnty + '"/>' + 										
                                    '</span>' +															
                                        '<span class="busyImage" style="z-index: 1000; position: absolute; display: none; margin-left: -30px;">' +
                                            '<img src="/images/roller.gif" />' +
                                        '</span>' +
                                        '<a class="baynote btn btnSecondary btnSmall leftLoading ajaxAddToCart trolley omTagEvt" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" data-rel="' + theProduct.SKU + 
                                        '" data-prop="eVar8" data-om="BN-Add-'+ theProduct.SKU + '" '+
                                        ' href="javascript:void(0);">' +
                                            '<span class="pfTrolleyWhiteSmall">'+'</span>'+bnBuyText() +
                                        '</a>' +                                           
                                    '</div>' +
                                '</div>' +
                            '</div>' +
                            hideBlockBot1 +
                        '</div>' +
                        '<div class="foot"></div>' +
                    '</div>' +
                '</div>';
    return prodSource;
}

function normal_product_results_html(widget, icsuffix) {
    var placeholderName, displayName, name;
    placeholderName = widget.placeholderName;
    displayName = widget.displayName;
    name = widget.name;
    var prodSource = "";
    prodSource += '<div id="bn_guidearea' + placeholderName + '" class="bn_g_area custBought baynote">' +
                '<div id="bn_guidewelcome' + placeholderName + '" class="title bn_g_welcome"><h2>' + displayName + '</h2></div>';
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += vertical_html(recData[i], icsuffix, placeholderName, name);
    }
    prodSource += '</div>';
    /*if (document.getElementById("bn_level5_4NP") && placeholderName == "bn_level5_3NP" ) {placeholderName = "bn_level5_4NP";}*/
    if (document.getElementById(placeholderName)) document.getElementById(placeholderName).innerHTML = prodSource;
}

function normal_results_html(widget) {
    var prodSource, placeholderName, displayName;
    prodSource = "";
    placeholderName = widget.placeholderName;
    displayName = widget.displayName;
    prodSource += '<div id="bn_guidearea' + placeholderName + '" class="bn_g_area custBought">' +
                '<div id="bn_guidewelcome' + placeholderName + '" class="title bn_g_welcome"><h2>' + displayName + '</h2></div>';
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += vertical_html(recData[i],"", placeholderName, widget.name);
    }
    prodSource += '</div>';
    if (document.getElementById(placeholderName)) document.getElementById(placeholderName).innerHTML = prodSource;
}

function horizontal_html(recData, icsuffix, placeholderName, name) {
    var theProduct = mapRec(recData);	
    var noPrice = theProduct.Price > 0 ? false : true;
    var hideBlockTop="";
    var hideBlockBot = "";
    var hideBlockTop1 ="";
    var hideBlockBot1 = "";  
    
    if (noPrice) {
        hideBlockTop = '<div class="mk-tab-pane">';
        hideBlockBot = '</div>';
    }
    if (bnLocale.indexOf("en-sg") > -1 || bnLocale.indexOf("en-au") > -1 )
    {
    }        
    else
    {
        hideBlockTop1 = hideBlockTop;
        hideBlockBot1 = hideBlockBot;
    }
        
    var prodSource = "";
    prodSource += '<div id="bn_g_result' + placeholderName + '_' + theProduct.slot + '" class="bn_g_result bn_g_result">' +    	
                '<div class="cwbt-content" align="center">' +
                    '<br />';
    if (theProduct.ThumbUrl) {
        prodSource += 	'<div class="imgContainer printShow">' +
                        '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="' + iclink1(theProduct.seoURL, theProduct.SKU, icsuffix) + '">' +
                            thumbnail(theProduct.ThumbUrl.replace("thumbnail","standard"),"110px") +
                        '</a>' +                     
                    '</div>';
    }
    prodSource += 		'<div class="cwbtdesc" style="height:90px;padding-bottom:10px">' +
                        '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="' + iclink1(theProduct.seoURL, theProduct.SKU, icsuffix) + '">' +
                                '<div class="brandName">' +
                                    theProduct.Manufacturer +
                                '</div>' +
                                '<div class="manuSku">' +
                                    theProduct.MfrPartNumber +
                                '</div>' +	
                        '</a>' +                      
                        '<div class="description">' +
                            theProduct.ProductName +
                            '<span class="mk-tab-pane">' +
                                '<a data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" href="SKU=' + theProduct.SKU + '"></a>' +
                            '</span>' +							
                        '</div>' +
                        hideBlockTop+
                        '<div class="merchPrice1 ">' +
                            '<p class="priceFor"  style="margin-bottom:2px;float:left">' +
                                '<strong>'+bnTextPrice()+'&nbsp;</strong>' + uom(theProduct.UOM) + '&nbsp;</p>' +
                            '<p class="price"  style="margin-bottom:2px;float:left">' +
                                minOrder()+'&nbsp;' + theProduct.MinOrderQnty + '+&nbsp;' +
                                '<strong>' + bnPrice(theProduct.Price) + '</strong>' +
                            '</p>' +			
                        '</div>' +
                        hideBlockBot +						
                    '</div>' +
                    hideBlockTop1+
                    '<div class="mktBuyLine">' +
                        '<br />' +
                        '<div class="buy_button, buyLine mk-tab-pane"></div>' +
                        '<div class="mktBuy_button mktBuyLine">' +
                            '<span class="mk-tab-pane">' +
                                '<input value="' + theProduct.MinOrderQnty + '" name="quantity_' + theProduct.SKU + '" data-min="' + theProduct.MinOrderQnty + '"/>' + 										
                            '</span>' +													
                            '<div class="merchButton" style="float:right">' +
                                '<span class="busyImage" style="z-index: 1000; position: absolute; display: none; margin-left: -30px;">' +
                                    '<img src="/images/roller.gif" />' +
                                '</span>' +
                                '<a class="baynote btn btnSecondary btnSmall leftLoading ajaxAddToCart mkOmTagEvt trolley " data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '" title="' + theProduct.ProductName + '" data-rel="' + theProduct.SKU +
                                '" data-prop="eVar8" data-om="BN-Add-'+ theProduct.SKU + '" '+
                                ' href="javascript:void(0);">' +bnBuyText()+
                                    '<span class="pfTrolleyWhiteSmall">'+'</span>' +
                                '</a>' +                                           
                            '</div>' +
                        '</div>' +
                    '</div>' +
                    hideBlockBot1+
                '</div>' +
            '</div>';
    return prodSource;
}

function normal_shopping_results_html(widget, icsuffix) {
    var placeholderName, displayName, name;
    placeholderName = widget.placeholderName;
    displayName = widget.displayName;
    name = widget.name;
    var prodSource = "";
    prodSource += '<div id="bn_guidearea' + placeholderName + '" class="bn_g_area custBought">' +
                '<div id="bn_guidewelcome' + placeholderName + '" class="title bn_g_welcome"><h2>' + displayName + '</h2></div>';
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += horizontal_html(recData[i], icsuffix, placeholderName, name);
    }
    prodSource += '</div>';
    document.getElementById(placeholderName).innerHTML = prodSource;
}

function cart_html(widget) {
    var placeholderName, displayName;
    placeholderName = widget.placeholderName;
    displayName = widget.displayName;
    var prodSource = "";
    if (document.getElementById("bn_cart_1_test") && placeholderName == "bn_cart_1" ) {placeholderName = "bn_cart_1_test";}        
    prodSource += '<div id="bn_guidearea' + placeholderName + '" class="bn_g_area">' +
                '<div class="cwbtheader-out">' +
                    '<div id="bn_guidewelcome' + placeholderName + '" class="cwbtheader-in bn_g_welcome">' +
                        displayName +
                    '</div>' +
                '</div>';
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += horizontal_html(recData[i], null, placeholderName, widget.name);
    }
    prodSource += '</div>';
    document.getElementById(placeholderName).innerHTML = prodSource;
}

function view_history(recData, trackingCode, placeholderName, name) {
    var iclink, url, slot, ThumbUrl, SKU, ProductName, Manufacturer, MfrPartNumber,ParentCategory,SubCategory,seoURL,UOM,MinOrderQnty,Price;
    url = recData.url;
    slot = recData.slot;
    iclink = url + "&MER=" + trackingCode + slot;
    var attrs = recData.attrs;
    for (var j = 0; j < attrs.length; j++) {
        switch (attrs[j].name) {
            case "ThumbUrl":
                ThumbUrl = attrs[j].values[0];
                break;
            case "SKU":
                SKU = attrs[j].values[0];
                break;
            case "ProductName":
                ProductName = attrs[j].values[0];
                break;
            case "seoURL":
                seoURL = attrs[j].values[0];
                break;
            case "Manufacturer":
                Manufacturer = attrs[j].values[0];
                break;
            case "MfrPartNumber":
                MfrPartNumber = attrs[j].values[0];
                break;
            case "MinOrderQnty":
                MinOrderQnty = attrs[j].values[0];
                break;
            case "UOM":
                UOM = attrs[j].values[0];
                break;
            case "ParentCategory":
                ParentCategory = attrs[j].values[0];
                break;				
            case "SubCategory":
                SubCategory = attrs[j].values[0];
                break;				
            case "Price":
                Price = Number(attrs[j].values[0]).formatMoneyBN(4);
                break;
        }
    }
    var prodSource = "";
    prodSource += '<div class="item separator">' +
                '<div class="com300">' +
                    '<div class="head"></div>' +
                    '<div id="bn_g_result' + placeholderName + '_' + slot + '" class="content">';
    if (ThumbUrl) {
        prodSource += 		'<div class="image">' +
                            '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + slot + '" data-baynote-pid="' + SKU + '" title="' + ProductName + '" href="' + iclink + '">';
        if (ThumbUrl.indexOf("<img") != -1) {
            prodSource += ThumbUrl;
        } else if (ThumbUrl.indexOf("http:") != -1){
            prodSource += 			'<img src="' + ThumbUrl.split("http:")[1] + '"/>';
        }
        else {
            prodSource += 			'<img src="' + ThumbUrl + '"/>';
        }
        prodSource += 			'</a>' +
                        '</div>';
    }
    prodSource += 			'<div class="part_detail with_image">' +
                            '<div class="part_info">' +
                                '<div class="description">' +
                                    '<b><a class="bn_g_result_link" data-baynote-widget="' + name + '" data-baynote-slot="' + slot + '" data-baynote-pid="' + SKU + '" title="' + ProductName + '" href="' + iclink + '">' + ProductName + '</a></b>' +
                                '</div>' +
                                '<div class="mfr_info">' +
                                    '<b>' + Manufacturer + '</b>' +
                                    '<br />' +
                                    MfrPartNumber +
                                '</div>' +	
                            '</div>' +
                        '</div>' +
                    '</div>' +
                    '<div class="foot"></div>' +
                '</div>' +
            '</div>';
    return prodSource;
}

function browse_html(widget) {
    var placeholderName, displayName;
    placeholderName = widget.placeholderName;
    displayName = widget.displayName;
    var prodSource = "";
    prodSource += '<div id="bn_guidearea' + placeholderName + '" class="bn_g_area">' +
                '<div class="cwbtheader-out">' +
                    '<div id="bn_guidewelcome' + placeholderName + '" class="cwbtheader-in bn_g_welcome">' +
                        displayName +
                    '</div>' +
                '</div>';
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += view_history(recData[i], "bn-me-ca-r3-reco-all-", placeholderName, widget.name);
    }
    prodSource += '</div>';
    document.getElementById(placeholderName).innerHTML = prodSource;
}

function best_selling_products(widget) {
    var prodResult = "";
    var name = widget.name;
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        var url, SKU, iclink, ParentCategory, slot, ProductName, ThumbUrl, MfrPartNumber, Manufacturer,SubCategory;
        url = recData[i].url;
        slot = recData[i].slot;
        var attrs = recData[i].attrs;
        for (var j = 0; j < attrs.length; j++) {
            switch (attrs[j].name) {
            case "ThumbUrl":
                ThumbUrl = attrs[j].values[0];
                break;
            case "SKU":
                SKU = attrs[j].values[0];
                break;
            case "ProductName":
                ProductName = attrs[j].values[0];
                break;
            case "seoURL":
                seoURL = attrs[j].values[0];
                break;
            case "Manufacturer":
                Manufacturer = attrs[j].values[0];
                break;
            case "MfrPartNumber":
                MfrPartNumber = attrs[j].values[0];
                break;
            case "MinOrderQnty":
                MinOrderQnty = attrs[j].values[0];
                break;
            case "ParentCategory":
                ParentCategory = attrs[j].values[0];
                break;				
            case "SubCategory":
                SubCategory = attrs[j].values[0];
                break;								
            case "UOM":
                UOM = attrs[j].values[0];
                break;
            case "Price":
                Price = Number(attrs[j].values[0]).formatMoneyBN(4);
                break;
            }
        }
        iclink = url + "&MER=bn-me-la-mi-67888-" + SKU;
        if (i % 5 == 0) {
            prodResult += '<div class="mk-espot-box2-text mk-espot-5-prod">' +
                        '<div class="mk-espot-base">' +
                            '<div class="mk-espot-base-row">';
        }
        prodResult += '<div class="mk-espot-cell mk-espot-base-width">' +
                    '<div class="mk-espot-manu-logo mk-leftmargin-12"><strong>' + ParentCategory + '</strong></div>' +
                        '<div class="mk-espot-product-image">' +
                            '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + slot + '" data-baynote-pid="' + SKU + '" title="' + ProductName + '" href="' + iclink + '">';
        if (ThumbUrl.indexOf("<img") != -1) {
            prodResult += ThumbUrl;
        } else if (ThumbUrl.indexOf(",") != -1) {
            prodResult += '<img src="' + ThumbUrl.split(",")[0] + '" alt="' + MfrPartNumber + '" />';
        } else if (ThumbUrl.indexOf("http:") != -1){
            prodResult += '<img src="' + ThumbUrl.split("http:")[1] + '" alt="' + MfrPartNumber + '" />';
        }
        else
        {
        prodResult += '<img src="' + ThumbUrl + '" alt="' + MfrPartNumber + '" />';
        }
        prodResult += 			'</a>' +
                        '</div>' +
                        '<div class="mk-espot-text">' +
                            '<div class="mk-espot-product-description"><strong>' +
                                '<a class="bn_g_result_image_link" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + SKU + '" title="' + ProductName + '" href="' + iclink + '">';
        if (ProductName.indexOf("|") != -1) {
            if (ProductName.indexOf(MfrPartNumber + " - " + Manufacturer + " - ") != -1) {
                prodResult += ProductName.split(MfrPartNumber + " - " + Manufacturer + " - ")[1].split("|")[0].trim();
            } else {
                prodResult += ProductName.split("|")[0].trim();
            }
        } else {
            prodResult += ProductName;
        }
        prodResult += 				'</a>' +
                            '</strong></div>' +
                            '<div class="mk-espot-mfr-info"><b>' + Manufacturer + '</b><br />' +
                                MfrPartNumber +
                            '</div>' +
                            '<a href="' + iclink + '" class="mk-primary-btn mk-btn5" data-baynote-widget="' + name + '" data-baynote-slot="' + slot + '" data-baynote-pid="' + SKU + '" title="' + ProductName + '" >'+bnBuyText()+'</a>' +
                        '</div>' +
                    '</div>';
        if (i % 5 == 4) {
            prodResult += '</div></div></div>';
        }
    }
    document.getElementById(widget.placeholderName).innerHTML = prodResult;
}

function nwp_hp_cell(recData, name, manu) {
    var prodSource = "";
    var theProduct = mapRec(recData);
    borderRight = (theProduct.slot + 1) % 5 == 0 ? "" : "mk-border-right";

    if (theProduct.seoURL.indexOf("?") > -1 ) {iclink = theProduct.seoURL + "&MER=bn-me-hp-mi" + manu + theProduct.SKU}
    else {
    iclink = theProduct.seoURL + "?MER=bn-me-hp-mi" + manu + theProduct.SKU;
    }
    prodSource += '<a href="' + iclink + '" class="mk-espot-base-width mk-espot-4cell ' + borderRight + '" data-baynote-widget="' + name + '" data-baynote-slot="' + theProduct.slot + '" data-baynote-pid="' + theProduct.SKU + '">' +
                '<div class="mk-espot-image1-carrousel">';
    if (theProduct.ThumbUrl.indexOf("<img") != -1) {
        prodSource += theProduct.ThumbUrl;
    } else if (theProduct.ThumbUrl.indexOf(",") != -1) {
        prodSource += '<img src="' + theProduct.ThumbUrl.split(",")[0] + '" alt="' + theProduct.MfrPartNumber + '" />';
    } else if (theProduct.ThumbUrl.indexOf("http") != -1) {
        prodSource += '<img src="' + theProduct.ThumbUrl.split("http:")[1] + '" alt="' + theProduct.MfrPartNumber + '" />';
    }
    else {
        prodSource += '<img src="' + theProduct.ThumbUrl+ '" alt="' + theProduct.MfrPartNumber + '" />';
    }

    prodSource += 	'</div>' +
                '<div class="mk-espot-supplier">' +
                    '<p>' + theProduct.Manufacturer + '</p>' +
                '</div>' +
                '<div class="mk-col-100 mk-espot-text-carrousel">' +
                    '<p class="mk-pointer1 mk-bottommargin-12">';
    if (theProduct.ProductName.indexOf("|") != -1) {
        if (theProduct.ProductName.indexOf(theProduct.MfrPartNumber + " - " + theProduct.Manufacturer + " - ") != -1) {
            prodSource += theProduct.ProductName.split(theProduct.MfrPartNumber + " - " + theProduct.Manufacturer + " - ")[1].split("|")[0].trim();
        } else {
            prodSource += theProduct.ProductName.split("|")[0].trim();
        }
    } else {
        prodSource += theProduct.ProductName;
    }
    prodSource += 		'</p>' +
                '</div>' +
            '</a>';
    return prodSource;
}

function home_html(widget, manu) {
    var name = widget.name;
    var prodSource = "";
    if (name == "NWP_HP_RecentHistory") {
        prodSource += '<div class="mk-pullout-box1 mk-col-100 mk-secondary"><div class="mk-pullout-titlebar"><h3>Your recently viewed history</h3></div></div>' +
                    '<div class="mk-espot-box2-text">' +
                        '<div class="mk-espot-base mk-subtle">' +
                            '<div class="mk-mod8">' +
                                '<div class="mk-espot-base-row mk-baynote">';
    } else {
        prodSource += '<div class="mk-espot-box2-text">' +
                    '<div class="mk-espot-base mk-subtle">' +
                        '<div class="mk-mod8">' +
                            '<div class="mk-espot-base-row mk-baynote">';
    }
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        prodSource += nwp_hp_cell(recData[i], name, manu);
    }
    if (name == "NWP_HP_RecentHistory" && !widget.recData[3].url) {
            prodSource += '<div class="mk-espot-base-width mk-espot-2cell">&nbsp;</div>';
    }
    prodSource += '</div></div></div></div>';
    document.getElementById(widget.placeholderName).innerHTML = prodSource;
}

function hp_recommendation_html(widget, manu) {
    var prodSource = "";
    var recData = widget.recData;
    for (var i = 0; i < recData.length && recData[i].url; i++) {
        var slot, name;
        slot = recData[i].slot;
        name = widget.name;
        var borderBottom = slot > 12 ? "" : "mk-hp-bottom-border";
        if (slot % 4 == 1) {
            prodSource += '<div class="mk-espot-box2-text">' +
                        '<div class="mk-espot-base mk-subtle">' +
                            '<div class="mk-mod8 ' + borderBottom + '">' +
                                '<div class="mk-espot-base-row mk-baynote">';
        }
        prodSource += nwp_hp_cell(recData[i], name, manu);
        if (slot % 4 == 0) {
            prodSource += '</div></div></div></div>';
        }
    }
    document.getElementById(widget.placeholderName).innerHTML = prodSource;
}
/******* END: 'legacy' *******/

/** End - Render template styles */
/***********************************/

function bnGetCountryCode() {
	var bnCountryCodes = {
		"en-au":"Element14_Australia",
		"de-at":"Element14_Austria",
		"nl-be":"Element14_Belgium",
		"fr-be":"Element14_Belgium",
		"en-ca":"Element14_Canada",
		"zh-cn":"Element14_China",
		"cs-cz":"Element14_Czech_Republic",
		"da-dk":"Element14_Denmark",
		"fi-fi":"Element14_Finland",
		"fr-fr":"Element14_France",
		"de-de":"Element14_Germany",
		"hu-hu":"Element14_Hungary",
		"en-in":"Element14_India",
		"it-it":"Element14_Italy",
		"ko-kr":"Element14_South_Korea",
		"en-my":"Element14_Malaysia",
		"es-mx":"Element14_Mexico",
		"nl-nl":"Element14_Netherlands",
		"en-nz":"Element14_New_Zealand",
		"nb":"Element14_Norway",
		"en-ph":"Element14_Philippines",
		"pl-pl":"Element14_Poland",
		"ro-ro":"Element14_Romania",
		"en-sg":"Element14_Singapore",
		"es-es":"Element14_Spain",
		"sv-se":"Element14_Sweden",
		"de-ch":"Element14_Switzerland",
		"fr-ch":"Element14_Switzerland",
		"zh-tw":"Element14_Taiwan",
		"th-th":"Element14_Thailand",
		"en-gb":"Element14_United_Kingdom",
		"en-us":"Element14_US"
	};
	if (bnLocale) {
		var country = bnCountryCodes[bnLocale];
		return country || null;
	}
}

function bn_setCatalogDocAttrs() {
	var catValue = pf.tagging.data.omniture.prop13 || "";
	if (catValue) {
		var catArray = catValue.split(">");
		var catLeaf = catArray.length > 0 ? catArray[catArray.length - 1].trim() : "";
		if (catLeaf == "Product View") {
			return;
		}
		var products = document.getElementById("sProdList");
		if (!products) {
			products = document.getElementsByClassName("popularCategoryImg")[0];
		}
		if (products) {
			var imageElement = products.getElementsByTagName("img")[0];
			if (imageElement) {
				var image = imageElement.src;
			}
		}
		var entityUpdates = [
			{
				"name":"document",
				"id":location.href,
				"attr":"Title",
				"values":[document.title.split("|")[0].trim()]
			}, {
				"name":"document",
				"id":location.href,
				"attr":"categoryId",
				"values":[catLeaf]
			}, {
				"name":"document",
				"id":location.href,
				"attr":"categoryPageCategory",
				"values":[catLeaf]
			}, {
				"name":"document",
				"id":location.href,
				"attr":"categoryPageFullCategory",
				"values":[catValue]
			}, {
				"name":"document",
				"id":location.href,
				"attr":"PageType",
				"values":["category"]
			}


		];
		if (image) {
			entityUpdates.push({
				"name":"document",
				"id":location.href,
				"attr":"image",
				"values":[image]
			});
		}
		var country = bnGetCountryCode();
		if (country) {
			entityUpdates.push({
				"name":"document",
				"id":location.href,
				"attr":"countryCode",
				"values":[country]
			});
		}
		var SKU = pf.canonical.data.categoryId;
		if (SKU) {
			entityUpdates.push({
				"name":"document",
				"id":location.href,
				"attr":"SKU",
				"values":[SKU]
			});
		}
		bnObserver.registerEntityUpdates(entityUpdates);
	}
}

bnResultsPageType = null;

function myPreHandler(tag) {
    
    // Observer Pre-handler
    if (typeof tag != 'undefined' && typeof tag.type != 'undefined' && tag.type == bnConstants.OBSERVER_TAG) {

		//thor code
		var trailArray = [];
        var bnTrailParse = [];
        var bnTrail = BaynoteAPI.getCookieValue('bn_documentVisitsTrail');
        if(bnTrail) {
            bnTrailParse = bnCommon.parseJSON(bnTrail);
            trailArray   = bnTrailParse;
		if (trailArray.length > 3) {
			BaynoteAPI.removeCookie("bn_documentVisitsTrail");
		}
        }
        
        // exit handler
        tag.exitConfirmation = bn_onClickHandler;

        // set tag.url
        var tempURL = computePageURL();
        if (typeof (tempURL) != "undefined" && BaynoteAPI.isNotEmpty(tempURL)) {
            tag.url = tempURL;
        }
        /* Uncomment this line to enable eu events */
		// bn_setCatalogDocAttrs();
        if (BNThor.getInjectPolicy() === BNThor.Constants.THOR_INJECT_POLICY) {

            var element, requestDefinedAttrs = [],contextUrls, visitstrail;
/*
            if (location.hostname == "uk.farnell.com") {
                requestDefinedAttrs.push({"entityName":"Context","attrName":"country","attrValue":"Element14_United_Kingdom"});
            }

            if (location.hostname == "www.newark.com") {
                requestDefinedAttrs.push({"entityName":"Context","attrName":"country","attrValue":"Element14_US"});
            } else if (location.hostname == "canada.newark.com") {
                requestDefinedAttrs.push({"entityName":"Context","attrName":"country","attrValue":"Element14_Canada"});
            } else if (location.hostname == "mexico.newark.com") {
                requestDefinedAttrs.push({"entityName":"Context","attrName":"country","attrValue":"Element14_Mexico"});
            }
*/
            /* Start: 'mer' - Map resultsPageType and Placeholders */
            if ( typeof s !== "undefined" || s !== null ) {
                if ( typeof s.pageName !== "undefined" || s.pageName !== null ) {

                    var mer_pageName = s.pageName;
                    var mer_placeholders = {
                        "catNode":["bn_browse_1TP","bn_browse_3TP","bn_browse_4TP","bn_browse_2","BN_BrowseGuide"],
                        "catLeaf":["bn_para_1TP","bn_para_2TP"],
                        "searchResults":["bn_search_1TP","bn_search_2TP","bn_search_3TP"],
                        "productDetail":["bn_level5_3NP","bn_level5_4NP","bn_level5_5NP"],
                        "checkoutOrderConf":["bn_orderconf_1TP","bn_orderconf_2TP"]
                    };

                    if ( /Product\ Detail/.test(mer_pageName) ) { // ProductDetail
                        bnResultsPageType = "ProductDetail";
                        //console.log("bnResultsPageType",bnResultsPageType);
						if (location.hostname == "ch.farnell.com") {
                        	mer_setAttribute(mer_placeholders["productDetail"],"data-baynote-page-template","PT_ProductDetail_CH");
						} else if (location.hostname == "hu.farnell.com") {
                        	mer_setAttribute(mer_placeholders["productDetail"],"data-baynote-page-template","PT_ProductDetail_HU");
						} else if (location.hostname == "no.farnell.com") {
                        	mer_setAttribute(mer_placeholders["productDetail"],"data-baynote-page-template","PT_ProductDetail_NO");
						} else if (location.hostname == "ro.farnell.com") {
                        	mer_setAttribute(mer_placeholders["productDetail"],"data-baynote-page-template","PT_ProductDetail_RO");
						} else {
                        	mer_setAttribute(mer_placeholders["productDetail"],"data-baynote-page-template","PT_ProductDetail");
						}
                    } else if ( /Search|Category/.test(mer_pageName) ) { // Browse/Search
                        bnResultsPageType = "Category";
                        console.log("bnResultsPageType",bnResultsPageType);
                        if ( typeof pf !== "undefined" || pf !== null ) {
                            if (typeof pf.param !== "undefined" || pf.param !== null) {
                                if (typeof pf.param.pageMode !== "undefined" || pf.param.pageMode !== null) {
                                    if (pf.param.pageMode == "catnode") { // CatNode
										if (location.hostname == "ch.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catNode"],"data-baynote-page-template","PT_CatNode_CH");
										} else if (location.hostname == "hu.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catNode"],"data-baynote-page-template","PT_CatNode_HU");
										} else if (location.hostname == "no.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catNode"],"data-baynote-page-template","PT_CatNode_NO");
										} else if (location.hostname == "ro.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catNode"],"data-baynote-page-template","PT_CatNode_RO");
										} else {
                                        	mer_setAttribute(mer_placeholders["catNode"],"data-baynote-page-template","PT_CatNode");
										}
                                        //mer_setAttribute(mer_placeholders["catNode"],"data-baynote-page-template","PT_Browse")
                                    } else if (pf.param.pageMode == "catleaf") { // CatLeaf (Parametric)
										if (location.hostname == "ch.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catLeaf"],"data-baynote-page-template","PT_CatLeaf_CH");
										} else if (location.hostname == "hu.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catLeaf"],"data-baynote-page-template","PT_CatLeaf_HU");
										} else if (location.hostname == "no.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catLeaf"],"data-baynote-page-template","PT_CatLeaf_NO");
										} else if (location.hostname == "ro.farnell.com") {
                                        	mer_setAttribute(mer_placeholders["catLeaf"],"data-baynote-page-template","PT_CatLeaf_RO");
										} else {
                                        	mer_setAttribute(mer_placeholders["catLeaf"],"data-baynote-page-template","PT_CatLeaf");
										}
                                        //mer_setAttribute(mer_placeholders["catLeaf"],"data-baynote-page-template","PT_Browse")
                                    }
                                    else if (pf.param.pageMode == "search") { // Search
										if (location.hostname == "ch.farnell.com") {
											mer_setAttribute(mer_placeholders["searchResults"],"data-baynote-page-template","PT_SearchResults_CH");
										} else if (location.hostname == "hu.farnell.com") {
											mer_setAttribute(mer_placeholders["searchResults"],"data-baynote-page-template","PT_SearchResults_HU");
										} else if (location.hostname == "no.farnell.com") {
											mer_setAttribute(mer_placeholders["searchResults"],"data-baynote-page-template","PT_SearchResults_NO");
										} else if (location.hostname == "ro.farnell.com") {
											mer_setAttribute(mer_placeholders["searchResults"],"data-baynote-page-template","PT_SearchResults_RO");
										} else {
                                        	mer_setAttribute(mer_placeholders["searchResults"],"data-baynote-page-template","PT_SearchResults");
										}
                                        //mer_setAttribute(mer_placeholders["searchResults"],"data-baynote-page-template","PT_Browse")
                                    }
                                }
                            }
                        }

                        if (pf.param.pageMode == "catnode" || pf.param.pageMode == "catleaf" || pf.param.pageMode == "search") {

                            // Capture SubCategory or ParentCategory context
                            var aCategoryFilters = mer_getCategoryFilter();
                            //console.log("aCategoryFilters",aCategoryFilters);

                            if ( aCategoryFilters ) {
                                var catContext;
                                if (aCategoryFilters.length == 2) { // SubCategory
                                    catContext = aCategoryFilters[1].replace("&amp;", "&");
                                    //console.log("SubCategory catContext",catContext);
                                    requestDefinedAttrs.push({"entityName":"Context","attrName":"SubCategory","attrValue":catContext});
                                } else { // ParentCategory
                                    catContext = aCategoryFilters[0].replace("&amp;", "&");
                                    //console.log("ParentCategory catContext",catContext);
                                    requestDefinedAttrs.push({"entityName":"Context","attrName":"ParentCategory","attrValue":catContext});
                                }
                            }

                            //var parentCat = bn_setCategoryFilter() ? bn_setCategoryFilter().replace("&amp;", "&"): false;

                            /*if (parentCat) {
                                console.log("parentCat",parentCat);
                                requestDefinedAttrs.push({"entityName":"Context","attrName":"ParentCategory","attrValue":parentCat});
                            }*/

                            //capture 'category' query
                            var query = BaynoteAPI.getURLParam("Ntt") || BaynoteAPI.getURLParam("st");
                            if (!query) {
                                var regex = /^https?:\/\/[^\/?#;]+\/([^\/?#;&]+)($|[?#;&].*)/;
                                var match = regex.exec(location.href);
                                if (match != null) {
                                    query = match[1];
                                }
                            }

                            if (query) {
                                console.log("query",query);
                                query = decodeURIComponent(query);
                                requestDefinedAttrs.push({"entityName":"Context","attrName":"category","attrValue":query});
                            }
                        }


                    } else if ( /Order\ Confirmation/.test(mer_pageName) ) { // Order Confirmation
                        bnResultsPageType = "Cart";
						if (location.hostname == "ch.farnell.com") {
                        	mer_setAttribute(mer_placeholders["checkoutOrderConf"],"data-baynote-page-template","PT_OrderConfirm_CH");
						} else if (location.hostname == "hu.farnell.com") {
                        	mer_setAttribute(mer_placeholders["checkoutOrderConf"],"data-baynote-page-template","PT_OrderConfirm_HU");
						} else if (location.hostname == "no.farnell.com") {
                        	mer_setAttribute(mer_placeholders["checkoutOrderConf"],"data-baynote-page-template","PT_OrderConfirm_NO");
						} else if (location.hostname == "ro.farnell.com") {
							mer_setAttribute(mer_placeholders["checkoutOrderConf"],"data-baynote-page-template","PT_OrderConfirm_RO");
						} else {
                        	mer_setAttribute(mer_placeholders["checkoutOrderConf"],"data-baynote-page-template","PT_OrderConfirmation");
						}
                        //console.log("bnResultsPageType",bnResultsPageType);
                        contextUrls = mer_AddPurchasedProducts();
                        trailArray = [];
                    }

                }
            }

            /* TODO - Migrate cart context */
            if ((element = document.getElementById("bn_cart_1")) || (element = document.getElementById("bn_cart_1_test"))) {
				if (location.hostname == "ch.farnell.com") {
					element.setAttribute("data-baynote-page-template", "PT_Cart_CH");
				} else if (location.hostname == "hu.farnell.com") {
					element.setAttribute("data-baynote-page-template", "PT_Cart_HU");
				} else if (location.hostname == "no.farnell.com") {
					element.setAttribute("data-baynote-page-template", "PT_Cart_NO");
				} else if (location.hostname == "ro.farnell.com") {
					element.setAttribute("data-baynote-page-template", "PT_Cart_RO");
				} else {
                	element.setAttribute("data-baynote-page-template", "PT_Cart");
				}
                bnResultsPageType = "Cart";
                contextUrls = bn_AddShoppingCartProducts();
                trailArray = [];
            }

            /* End: 'mer' - Map resultsPageType and Placeholders */





            /* Legacy Landing context */
            if (element = document.getElementById("BN_best_selling_products")) {
                element.setAttribute("data-baynote-page-template", "PT_BestSellers");
                bnResultsPageType = "Landing";
            } else if (element = document.getElementById("BN_best_selling_products")) {
                element.setAttribute("data-baynote-page-template", "PT_BestSellers");
                bnResultsPageType = "Landing";
            } else if (element = document.getElementById("BN_HomePage1")) {
                element.setAttribute("data-baynote-page-template", "PT_Home");
                bnResultsPageType = "Landing";
            } else if (element = document.getElementById("BN_Storefront_Teconnectivity")) {
                element.setAttribute("data-baynote-page-template", "PT_Storefront_TE");
                bnResultsPageType = "Landing";
            } else if (element = document.getElementById("BN_Storefront_Multicomp")) {
                element.setAttribute("data-baynote-page-template", "PT_Storefront_Multi");
                bnResultsPageType = "Landing";
            }
            if (element = document.getElementById("BN_NWP_HP_Bestsellers_Automation")) {
                element.setAttribute("data-baynote-page-template", "PT_Home");
                bnResutlsPageType = "Home";
            } else if (element = document.getElementById("BN_NWP_HP_Bestsellers_Connectors")) {
                element.setAttribute("data-baynote-page-template", "PT_Home");
                bnResultsPageType = "Home";
            }

            /* Legacy PDP context */
            if ( !((bnLocale.indexOf("en-gb") > -1) || (bnLocale.indexOf("en-us") > -1)) ) {
                if (element = document.getElementById("bn_level5_1")) {
                    element.setAttribute("data-baynote-page-template", "PT_Product");
                    bnResultsPageType = "ProductDetail";
                }
                if (element = document.getElementById("bn_level5_2")) {
                    element.setAttribute("data-baynote-page-template", "PT_Product");
                    bnResultsPageType = "ProductDetail";
                }
                if (element = document.getElementById("bn_level5_3")) {
                    element.setAttribute("data-baynote-page-template", "PT_Product");
                    bnResultsPageType = "ProductDetail";
                }
                if (element = document.getElementById("BN_NWPCustAlsoView")) {
                    element.setAttribute("data-baynote-page-template", "PT_Product");
                    bnResultsPageType = "ProductDetail";
                }
            }
			var country = bnGetCountryCode();
			if (country) {
				requestDefinedAttrs.push({"entityName":"Context","attrName":"country","attrValue":country});
			}
            /** Render Contexts **/
            BNThor.retrieveRenderingContext({
                successFn:function(context){
                    console.log(bnMKVers()," Success: ", context);
                    BNData = context.rawResponse.trackingData;
                    var widgets = context.widgets;
                    var bnPageId = context.rawResponse.id || "";
                    mktBNWidgets = widgets;

                    /* start: mer - render contexts */
                    var mer_hoz = [
                        "PD_CustAlsoViewed","PD_LastViewed",
                        "PD_CustAlsoViewed_CH","PD_LastViewed_CH",
                        "PD_CustAlsoViewed_HU","PD_LastViewed_HU",
                        "PD_CustAlsoViewed_NO","PD_LastViewed_NO",
                        "PD_CustAlsoViewed_RO","PD_LastViewed_RO",
                        "CN_Bestsellers","CN_LastViewed",
                        "CN_Bestsellers_CH","CN_LastViewed_CH",
                        "CN_Bestsellers_HU","CN_LastViewed_HU",
                        "CN_Bestsellers_NO","CN_LastViewed_NO",
                        "CN_Bestsellers_RO","CN_LastViewed_RO",
                        "CL_LastViewed",
                        "CL_LastViewed_CH",
                        "CL_LastViewed_HU",
                        "CL_LastViewed_NO",
                        "CL_LastViewed_RO",
                        "SR_LastViewed",
                        "OC_AlsoBought",
                        "OC_AlsoBought_CH",
                        "OC_AlsoBought_HU",
                        "OC_AlsoBought_NO",
                        "OC_AlsoBought_RO"
                    ];
                    var mer_ver = [
                        "SR_BrowseGuide",
						"SR_BrowseGuide_CH",
						"SR_BrowseGuide_HU",
						"SR_BrowseGuide_NO",
						"SR_BrowseGuide_RO"
                    ];
                    var mer_bAddToCart = mer_hasPFMiniBasketObj();
                    //console.log("mer_bAddToCart",mer_bAddToCart);

                    /* end: mer - render contexts */

                    for (var i = 0; i < widgets.length; i++) {
                        if (widgets[i].recData[0].url ) {

                            if ((new RegExp(mer_hoz.join("|")).test(widgets[i].name))) {
                                //console.log("hoz",{"widget":widgets[i],"hasBuyBtn":true});
                                mer_widget_html({"widget":widgets[i],"hasBuyBtn":false,"bnPageId":bnPageId});
                            } else if ((new RegExp(mer_ver.join("|")).test(widgets[i].name))) {
                                //console.log("ver",{"widget":widgets[i],"hasBuyBtn":true});
                                mer_widget_html({"widget":widgets[i],"hasBuyBtn":false,"bnPageId":bnPageId});
                            }

                            /* Start - legacy contexts */
                            if (widgets[i].placeholderName === "BN_NWPCustAlsoView") { widgets[i].placeholderName = "bn_level5_2";}
                            switch (widgets[i].name) {
                                case "ProductGuide":
                                case "ProductGuideG":
                                case "ProductGuideBottom":
                                case "BrowseGuide":
                                case "BrowseGuideG":
                                case "Storefront_TE":
                                case "CustAlsoViewed":
                                    normal_results_html(widgets[i]);
                                    break;
                                case "Storefront_Multicomp":                                    
					            case "HP_Zone1":
									normal_shopping_results_html(widgets[i], "-EN-HZ1-");
									break;                                    
                                case "NWP_Echo":
                                case "NWP_Echo_copy":
                                case "NWP_Bestseller":
                                case "NWP_Bestsellers":
                                case "BN_NWPBestsellers" :
                                case "NWPCustAlsoView":
                                case "LastViewed" :
                                    normal_results_html(widgets[i]);
                                    break;
                                case "LastViewedBased" :
                                case "Trending" :
                                    normal_results_html(widgets[i]);
                                    break;
                                case "ShoppingCart":
								case "ShoppingCart_CH":
								case "ShoppingCart_HU":
								case "ShoppingCart_NO":
								case "ShoppingCart_RO":
                                case "ShoppingCartG":
                                    cart_html(widgets[i]);
                                    break;
                                case "BrowseGuide1":
                                    browse_html(widgets[i]);
                                    break;
                                case "best_selling_products":
                                case "best_selling_product":
                                    best_selling_products(widgets[i]);
                                    break;
                                case "NWP_HP_Bestsellers_Connectors":
                                    home_html(widgets[i], "-con-");
                                    break;
                                case "NWP_HP_Bestsellers_SemiDiscretes":
                                    home_html(widgets[i], "-sdi-");
                                    break;
                                case "NWP_HP_Bestsellers_Passives":
                                    home_html(widgets[i], "-pas-");
                                    break;
                                case "NWP_HP_Bestsellers_SwitchesRelays":
                                    home_html(widgets[i], "-swi-");
                                    break;
                                case "NWP_HP_Bestsellers_TM":
                                    home_html(widgets[i], "-tes-");
                                    break;
                                case "NWP_HP_Bestsellers_Cable":
                                    home_html(widgets[i], "-cab-");
                                    break;
                                case "NWP_HP_Bestsellers_Enclosures":
                                    home_html(widgets[i], "-enc-");
                                    break;
                                case "NWP_HP_Bestsellers_Electrical":
                                    home_html(widgets[i], "-ele-");
                                    break;
                                case "NWP_HP_Bestsellers_SemiTools":
                                    home_html(widgets[i], "-sto-");
                                    break;
                                case "NWP_HP_Bestsellers_SemiModules":
                                    home_html(widgets[i], "-smo-");
                                    break;
                                case "NWP_HP_Bestsellers_SemiICs":
                                    home_html(widgets[i], "-sic-");
                                    break;
                                case "NWP_HP_Bestsellers_Crystals":
                                    home_html(widgets[i], "-cry-");
                                    break;
                                case "NWP_HP_Bestsellers_Automation":
                                    home_html(widgets[i], "-aut-");
                                    break;
                                case "NWP_HP_Bestsellers_Tools":
                                    home_html(widgets[i], "-too-");
                                    break;
                                case "NWP_HP_Recommendation":
                                    hp_recommendation_html(widgets[i], "-reco-");
                                    break;
                                case "NWP_HP_RecentHistory":
                                    home_html(widgets[i], "-hist-");
                                    break;
                                /* End - legacy contexts */
                            }
                        }
                        else {
                            console.log(bnMKVers()," Rec Config Error ",  widgets[i].name,widgets[i].placeholderName,widgets[i]);
                        }
                    }
                    if (context.rawResponse.id != "PT_Home") {
                        bnObserver.impressionOccurred();
                    }     
        //visitsTrail set up
        var pageUrl = document.location.href;
        var match = pageUrl.match(/^https?:\/\/(.*\.com)\/.*dp\/([^?]*).*/);
        // need to convert the url to the format:
        // //http://www.baur.de/$2@Baur-BaurDe$4
        if(match != null && match != "" && match.length == 3){
        	addToVisitsTrail("http://" + match[1] + "/jsp/search/productdetail.jsp?SKU=" + match[2]);
        } else {
            var regex2 = "^https?://.*SKU=([^&]*).*";
            var match2 = pageUrl.match(regex2);
            if(match2 != null && match2 != ""){
                 addToVisitsTrail(pageUrl);
            }
        }               
                },
                errorFn:function(err){
                    console.log("Error:", err);
                },
                attrs:["ThumbUrl", "SKU", "ProductName", "Description","seoURL", "Manufacturer", "MfrPartNumber", "MinOrderQnty", "UOM", "Price","ParentCategory","SubCategory"],
                contextUrls:contextUrls,
                visitstrail:trailArray,
                requestDefinedAttrs:requestDefinedAttrs,
                query:query
            });

        }
    }

    // Guide Pre-handler
    if (typeof tag != 'undefined' && typeof tag.type != 'undefined' && tag.type == bnConstants.GUIDE_TAG) {
            bnPageInfo.referrer = null;
        // AB test logic suppression
        //var cnd = BaynoteAPI.getCondition();
        
            //show baynote recs if you are A
        
    /*	if( cnd.indexOf(".B") > 0) {
            // show premier farnell recs
            if( BaynoteAPI.isNotEmpty( getElementsByClassName('endMerchRelativeProduct', 'span') ) ){
                getElementsByClassName('endMerchRelativeProduct', 'span')[0].style.display= "inline";
            }
            // return only if you are B and bn_level5_2 div exists
            if( BaynoteAPI.isNotEmpty(document.getElementById('bn_level5_2')) ){
                return false;
            }
        }
        */
        
        //capture query
        var query = BaynoteAPI.getURLParam("Ntt");
        if (typeof query != "undefined" && query != "") {
            tag.query = query;
        } else {
            var regex = /^https?:\/\/[^\/?#;]+\/([^\/?#;&]+)($|[?#;&].*)/;
            var match = regex.exec(location.href);
            if (match != null)
                query = match[1];
            else {
                regex = new RegExp("^https?://.*newark\.com/webapp/wcs/stores/servlet/Search\?.*st=([^&]*).*");
                match = regex.exec(location.href);
                if (match) {
                    query = match[1];
                }
            }
            if (typeof query != "undefined" && query != "")
                tag.query = query.replace(/%20/g, "+");
        }
        
        // set tag.url
        var tempURL = computePageURL();
        if (typeof (tempURL) != "undefined" && BaynoteAPI.isNotEmpty(tempURL)) {
            tag.url = tempURL;
        }
       
        if( typeof bn_manufacturer != 'undefined' &&  BaynoteAPI.isNotEmpty(bn_manufacturer) ){
            tag.docAttrs.manufacturer = bn_manufacturer;
        }
       
        //set listUrl to send recent history
        // Needed?
        if (bn_isProduct()) {
            bn_setContext(tag);
        } else if (location.href.indexOf("AjaxOrderItemDisplayView") == -1) {
        } else {
            /*bn_ListShoppingCartProducts(tag);*/
        }
        
        bn_setCategoryFilter(tag);
        bn_setEngine(tag);
    }

    return true;
}

/** 'mer' postHandler **/
function mktBNpostHandler(tag) {
    /* Bind shopping basket buttons */
    if (mer_hasPFMiniBasketObj()) {
        //console.log("addEvents");
        pf.minibasket.addEvents();
    }

    /** R17: Instantiate each Product Detail Collapsible Widget **/
    // TODO - Remove 'devEnv' for Prod
    if ( bnResultsPageType == "ProductDetail" ) {
        if (typeof dojo !== "undefined" && typeof dijit !== "undefined") {
            var ele;
            ele = dijit.registry.byId("PD_LastViewed");
            if(typeof ele == "undefined") {
                ele = dojo.byId("PD_LastViewed");
                if (ele != null) {
                    dojo.parser.instantiate([dojo.byId("PD_LastViewed")]);
                }
            }
            ele = dijit.registry.byId("PD_CustAlsoViewed");
            if(typeof ele == "undefined") {
                ele = dojo.byId("PD_CustAlsoViewed");
                if (ele != null) {
                    dojo.parser.instantiate([dojo.byId("PD_CustAlsoViewed")]);
                }
            }
        }
    }

}

function myPostHandler(tag) {
    if ( typeof mktBNpostHandler !== "undefined") {
        mktBNpostHandler(tag);
    }
    return true;
}

// if the page is a staging environment, use thor for injection
if (useThorRecs()) {
    BNThor.setInjectPolicy(BNThor.Constants.THOR_INJECT_POLICY);
}
// otherwise, use IC
else {
    BNThor.setInjectPolicy(BNThor.Constants.IC_INJECT_POLICY);
}

function useThorRecs() {
    return true;
}

// register the event handler
baynote_globals.onBeforeTagShow.push(myPreHandler);
baynote_globals.onTagShow.push(myPostHandler);
bnResourceManager.registerResource(baynote_globals.ScriptResourceId);