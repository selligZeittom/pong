mkOnReady(function () {
  require(["dojo/_base/lang", "dojo/io-query", "dijit/Dialog", "dojo/dom-attr", "dojo/dom", "dojo/query", "dojo/dom-class", "dojo/on", "dojo/_base/array", "dojo/dom-style", "dojo/NodeList-traverse"],
    function (lang, ioQuery, Dialog, domAttr, dom, query, domClass, on) {

      var paramForm = 'ftfrom';
      var footerWrap = dom.byId('footer-lead-form');
      var postErrorValidateEvent;

      var successPara = {};
      var retrunURL;

      successPara[paramForm] = true;
      var l = window.location;

      var uri = l.search;
      var queryObject = ioQuery.queryToObject(uri.substring(1));

      if (queryObject[paramForm]) {
        showDialog();
        _setClass(footerWrap, true, 'footer-post-success');
      } else {
        retrunURL = l.protocol + '//' + l.host + l.pathname + '?' + ioQuery.objectToQuery(lang.mixin(queryObject, successPara));
        query('#mk-email-sign-up-form', 'footer')
          .children('input#retURL').attr('value', retrunURL)
          .end()
          .on('submit', function (e) {
            e.preventDefault();
            var emailInput = query('#sf-signup-email', this);
            var emailInputVal = emailInput.attr('value')[0];
            validEmailAddress(emailInputVal) ? this.submit() : showErrors(this,emailInput);
          });
      }

      function _setClass(el, add, classString) {
        if (add) {
          if (!domClass.contains(el, classString)) {
            domClass.add(el, classString);
          }
        } else {
          if (domClass.contains(el, classString)) {
            domClass.remove(el, classString);
          }
        }
      }
      function validEmailAddress(emailValue) {
        var emailTest = /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
        return emailTest.test(emailValue);
      }
      function updateError(el, error) {
        _setClass(el, error, 'has-error');
        _setClass(el, !error, 'has-success');
      }
      function showErrors(formWrapper, emailInput) {
        updateError(formWrapper, true);
        if (!postErrorValidateEvent) {
          postErrorValidateEvent = on(emailInput, 'keyup', function () {
            updateError(formWrapper, !validEmailAddress(emailInput[0].value));
          })
        }
      }
      function showDialog() {
        var closeText = domAttr.get(footerWrap, "data-close") || "close";
        var thanksText = domAttr.get(footerWrap, "data-thanks") || "Thank you. You have been added to our email list";
        var myDialog = new Dialog({
          style: "width: 400px"
        });
        myDialog.set("content", "<h2>" + thanksText + "</h2><a href='#' class='btn btnPrimary'>" + closeText + "</a>");
        myDialog.set("id", "footer-signup-feedback-modal");
        myDialog.show();
        query('.btnPrimary', this.domNode).on('click', function (evt) {
          evt.preventDefault();
          myDialog.hide();
        });
      }
    });
});





