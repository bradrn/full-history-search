var blacklist =
    [ "ilearn.mq.edu.au",
      "mq.okta.com",
      "mq-edu-web.t1cloud.com",
      "macquarie.zoom.us",
      "duckduckgo.com",
      "www.google.com",
      "google.com"
    ];

if (!blacklist.includes(document.location.hostname)) {
    // Adapted from https://stackoverflow.com/a/2579869/
    var walker = document.createTreeWalker(
        document.body, 
        NodeFilter.SHOW_TEXT | NodeFilter.SHOW_ELEMENT, 
        null, 
        false
    );

    var node = walker.nextNode();
    var trimmed;
    var fulltext = "";

    while (node) {
        // skip <script> etc.
        if (node.nodeName === "HEAD" || node.nodeName === "SCRIPT" || node.nodeName === "STYLE") {
            node = walker.nextSibling();
            if (!node) {
                // at the end of a tag, so need to get sibling of parent
                walker.parentNode();
                node = walker.nextSibling();
            }
            continue;
        }

        if (node.nodeType === Node.TEXT_NODE) {
            trimmed = node.nodeValue.trim();
            if (trimmed != "") {
                fulltext += trimmed + " ";
            }
        }

        node = walker.nextNode();
    }

    var xhr = new XMLHttpRequest();
    xhr.open("PUT", "http://127.0.0.1:38589/page", true);

    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xhr.onreadystatechange = function() {
        if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
            console.log("Full text saved successfully");
        } else {
            console.log("Full text not saved");
        }
    };

    xhr.send(JSON.stringify({url: window.location.href, title: document.title, body: fulltext}));
}
