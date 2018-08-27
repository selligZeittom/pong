(function() {
    var event_name = 'load';

    if(window.location.search.indexOf('pfoverride') === -1) {
        if (window.attachEvent) {
            window.attachEvent("on" + event_name, function () {
                event_function.call(window);
            });
        } else if (window.addEventListener) {
            window.addEventListener(event_name, event_function, false);
        }
    }

    function event_function(){
        console.debug("SUDO JS BUILD LOADER");
        require({
            packages: [
                { name: "dojo", location: "/wcsstore/AuroraB2BStorefrontAssetStore/js/dist/dojo" },
                { name: "dijit", location: "/wcsstore/AuroraB2BStorefrontAssetStore/js/dist/dijit" },
                { name: "dojox", location: "/wcsstore/AuroraB2BStorefrontAssetStore/js/dist/dojox" },
                { name: "sudo", location: "/wcsstore/ExtendedSitesCatalogAssetStore/cms/asset/js/common/sudo/releases/2018_16"}]
        }, ['sudo/main'], function(pfsudo){
            window.mk =  new pfsudo();
            mk.init(pf, dojo);
        });
    }
}());
