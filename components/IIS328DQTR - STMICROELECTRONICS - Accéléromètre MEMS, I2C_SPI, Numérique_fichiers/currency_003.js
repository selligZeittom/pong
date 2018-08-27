/**
 * Created on 15/07/2016.
 * assets/js/common/modules/Currency
 *
 * modules/currency is an abstraction of dojo/currency which caters for
 * what is probably considered non-standard currency formatting. The purpose
 * being to assign the correct visual representation of price on the web
 * inline with the formats we currently use.
 *
 * In tandem with this dojo/currency does not behave well when trying to
 * parse our on page prices without some modification through parameters.
 * This work has now been centralised in to this module providing a simple
 * interface to access currency parsing (10.123,23 € > 10123.23)
 * and formatting (10123.23 > 10.123,23 €)
 *
 * formatCurrency: Take currency formatted strings and converts them
 * floats for transmitting taking in to account  regional display differences.
 *
 *  e.g.
 *  € 10,000.10
 *  €10.000,10
 *  10.000,10 €
 *  $10,000.10
 *
 */
define("sudo/modules/currency", ["dojo/_base/declare","dojo/currency"], function(declare, currency){
    "use strict";
    return (function(locale) {
        var currencyParams, getCurrencyParams, formatCurrency, parseCurrency;
        getCurrencyParams = function(locale) {
            /* Return appropriate currency currency and pattern */
            var params = {};
            params.places = "2,4";
            switch (locale) {
                case "nl-nl":
                case "de-at":
                    params.currency = "EUR";
                    params.pattern = "¤ #,##0.00";
                    params.locale =  "de";
                    break;
                case "es-es":
                case "de-de":
                    params.pattern = "#,##0.00 ¤";
                    params.currency = "EUR";
                    params.locale =  "de";
                    break;
                case "de-ch":
                case "fr-ch":
                    params.currency = "CHF";
                    params.pattern = "¤ #,##0.00";
                    break;
                case "sl-si":
                case "tr-tr":
                    params.currency = "EUR";
                    params.pattern = "¤ #,##0.00";
                    params.locale =  "de";
                    break;
                case "lt-lt":
                case "lv-lv":
                case "ru-ru":
                case "sk-sk":
                    params.currency = "EUR";
                    params.pattern = "#,##0.00 ¤";
                    params.locale = "fr";
                    break;
                case "da-dk" :
                    params.currency = "DKK";
                    params.pattern = "kr #,##0.00";
                    params.locale = "de";
                    break;
                case "ro-ro" :
                    params.currency = "RON";
                    params.pattern = "#,##0.00 lei";
                    params.locale = "es";
                    break;
                case "nb" :
                    params.currency = "NOK";
                    params.pattern = "kr #,##0.00";
                    params.locale = "fr";
                    break;
                case "sv-se" :
                    params.currency = "SEK";
                    params.pattern = "#,##0.00 kr";
                    params.locale = "fr";
                    break;
                case "bg-bg":
                case "et-ee" :
                    params.currency = "EUR";
                    params.pattern = "€#,##0.00";
                    params.locale = "fr";
                    break;
                case "cs-cz" :
                    params.currency = "CZK";
                    params.pattern = "#,##0.00 Kč";
                    params.locale = "fr";
                    break;
                case "hu-hu" :
                    params.currency = "HUF";
                    params.pattern = "#,##0.00 Ft";
                    params.locale = "fr";
                    break;
                case "fr-fr" :
                    params.currency = "EUR";
                    params.pattern = "#,##0.00 ¤";
                    params.locale = "fr";
                    break;
                case "it-it":
                    params.currency = "EUR";
                    params.pattern = "¤ #,##0.00";
                    params.locale = "de";
                    break;
                case "en-ie":
                    params.currency = "EUR";
                    break;
                case "fr-be":
                case "nl-be":
                case "pt-pt":
                    params.currency = "EUR";
                    params.pattern = "#,##0.00 ¤";
                    params.locale = "de";
                    break;
                case "fi-fi":
                    params.currency = "EUR";
                    params.pattern = "#,##0.00 ¤";
                    params.locale = "fr";
                    break;
                case "pl-pl":
                    params.currency = "PLN";
                    params.pattern = "#,##0.00 zł";
                    params.locale = "fr";
                    break;
                case "en-gb":
                    params.currency = "GBP";
                    break;
                case "en-au":
                    params.currency = "AUD";
                    params.pattern = "";
                    break;
                case "en-nz":
                    params.currency = "NZD";
                    params.pattern = "";
                    break;
                case "en-ph":
                    params.currency = "PHP";
                    break;
                case "th-th":
                    params.currency = "THB";
                    break;
                case "zh-tw":
                    params.currency = "NT$";
                    params.pattern = "¤#,##0.00";
                    break;
                case "en-ca":
                    params.currency = "CAD";
                    break;
                case "pt-br":
                    params.currency = "BRL";
                    params.pattern = "$ #,##0.00";
                    break;
                case "en-us":
                case "en-il":
                    params.currency = "USD";
                    break;
                case "es-mx":
                    params.currency = "MXN";
                    break;
                case "zh-cn":
                    params.currency = "CNY";
                    break;
                case "en-in":
                    params.currency = "INR";
                    params.pattern = "Rs.#,##0.00";
                    break;
                case "ko-kr":
                    params.currency = "KRW";
                    params.places = "0";
                    break;
                case "en-my":
                    params.currency = "MYR";
                    params.pattern = "";
                    break;
                case "en-sg":
                    params.currency = "SGD";
                    break;
                case "en-hk":
                    params.currency = "HKD";
                    break;
                default :
                    params.pattern = "USD";
            }
            return params;
        };
        currencyParams = getCurrencyParams(locale);
        parseCurrency = function(/* String formatted price*/ price, opt_params) {
            var result, params;
            params = opt_params || currencyParams;

            /*
            * Generates a new price string with
            * the correct number of decimal places
            * */

            result = currency.parse(price, params);

            // TODO: Retrace reasoning behind Number(??)
            return (!isNaN(result)) ? result : Number(price.replace(/[^0-9\.]+/g,""));

        };
        formatCurrency = function(/* String formatted price*/ price, opt_params) {
            var params;

            params = opt_params || currencyParams;

            /*
             * Generates a new price string with
             * the correct number of decimal places
             * */

            return currency.format(price, params);

        };
        return {
            _locale: locale,
            _getCurrencyParams: getCurrencyParams,
            _currencyParams: currencyParams,
            currency: currencyParams.currency,
            formatCurrency: formatCurrency,
            parseCurrency: parseCurrency
        };
    })(dojo.locale);
});
