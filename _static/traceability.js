// jQuery is included via the sphinxcontrib-jquery extension that we depend on
// item-tree
jQuery(function () {
    $('ul.bonsai').bonsai();
});

$(document).ready(function () {
    const anchorId = location.hash.slice(1);
    $('div.collapsible_links div.admonition.item').each(function (i) {
        $(this).siblings('dl').first().addCollapseButton($(this), anchorId);
        $(this).css('clear', 'left');  // sphinx-rtd-theme==0.5.0 sets `clear: both` which pushes button out of bar
    });

    // show an item's hidden caption on hover
    $('em.has_hidden_caption').each(function (i) {
        var caption = $(this).children('.popup_caption').first();
        var tableCell = caption.parents('td').first();
        // get background color of table cell, body, or white (no transparency allowed)
        var backgroundColor = tableCell.css('background-color');
        if ((typeof backgroundColor == 'undefined') || (backgroundColor == 'rgba(0, 0, 0, 0)')) {
            backgroundColor = $('body').css('background-color');
            if (typeof backgroundColor == 'undefined') {
                backgroundColor = 'white';
            }
        }
        caption.hide();  // hide on page load
        caption.css({
            'position': 'absolute',  // prevents allocation of space for caption
            'color': 'black',
            'background-color': backgroundColor,
            'padding': '3px',
            'font-weight': 'normal',  // prevents bold font in table header
            'z-index': '100',  // ensures that caption is on foreground
            'white-space': 'pre'  // prevents adding newlines
        });
        $(this).hover(
            function() {
                // entering hover state
                caption.show();
                var captionRight = caption.offset().left + caption.outerWidth();
                var container = $('div.rst-content')
                var maxRight = container.offset().left + container.innerWidth();
                if (captionRight > maxRight) {
                    // prevents overflow of container
                    var overflow = maxRight - captionRight;
                    caption.css('transform', 'translate(' + overflow + 'px, ' + '-1.5rem)');
                } else {
                    // lines up the caption behind the item ID
                    caption.css('transform', 'translate(0.3rem, -3px)');
                }
            }, function() {
                // leaving hover state
                caption.css('transform', 'none');  // resets the transformations
                caption.hide();
            }
        );
    });

    $('p.admonition-title').each(function (i) {
        $(this).children('a').first().denyPermalinkStyling($(this));
    });

    // if an item was selected, ensure it's displayed at the top of the viewport
    if (anchorId) {
        const element = document.getElementById(anchorId);
        if (element) {
            element.scrollIntoView(true, { block: "start", inline: "nearest" });
        }
    }
});

// item
jQuery.fn.extend({
    addCollapseButton: function (admonition, anchorId) {
        var relations = $(this);

        if (relations.children().length > 0) {
            const itemDiv = admonition.parent()
            if (itemDiv.hasClass('collapse') && (itemDiv.attr('id') != anchorId)) {
                // collapse relations and attributes list for each item on page load
                relations.toggle();
                arrowDirection = 'down';
            } else {
                arrowDirection = 'up';
            }

            var linkColor = admonition.children('p').first().css('color');
            var paddingX = '1px'
            if (admonition.css('padding') != '0px') {
                paddingX = '0.5rem'
            }
            var arrow = $('<i>', {
                class: "fa fa-angle-" + arrowDirection,
                css: {
                    'font-size': '135%',
                    'color': linkColor,
                    'float': 'right',
                    'padding': '1px ' + paddingX,
                    'cursor': 'pointer',
                },
                click: function () {
                    relations.toggle('fold');
                    $(this).toggleClass("fa-angle-up fa-angle-down");
                }
            });
            admonition.before(arrow);
        }
    },

    denyPermalinkStyling: function (admonition) {
        $(this).css("color", admonition.css("color"));
        $(this).css("text-decoration", admonition.css("text-decoration"));
        $(this).children('em').first().css("font-style", admonition.css("font-style"));
    }
});

window.addEventListener(
    "hashchange",
    () => {
        const anchorId = location.hash.slice(1);
        const element = document.getElementById(anchorId);
        if (element.classList.contains('collapse')) {
            for (const child of element.children) {
                if ((child.localName == 'i') && child.classList.contains('fa-angle-down')) {
                    child.click();
                    break;
                }
            }
        }
    },
);
