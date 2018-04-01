var langTools = ace.require("ace/ext/language_tools");
var editor = ace.edit("editor");    
editor.session.setMode("ace/mode/javascript");
// enable autocompletion and snippets
editor.setOptions({
    enableBasicAutocompletion: true,
    enableSnippets: false
});
editor.setShowPrintMargin(false);
editor.setValue('');

// Create tern server
var TernServer = ace.require("ace/tern/server").TernServer;
var defs = [ternEcma5Def];
var ternServer = new TernServer({defs: defs});
    
// Tern Completion
langTools.addCompleter(ternServer);

// Tern Tooltip
var TernTooltip = ace.require("ace/tern/tern_tooltip").TernTooltip;
editor.ternTooltip = new TernTooltip(editor, ternServer);

(function(window, undefined){

    /*
    setTimeout(function(){

        var _elements = document.getElementsByClassName("ace_scrollbar");
        
        for (var i = 0; i < _elements.length; i++)
        {
            _elements[i].style.overflow = "hidden";
            Ps.initialize(_elements[i], {
                theme : 'custom-theme'
            });
        }
        
    }, 100);

    function updateScroll()
    {
        var _elements = document.getElementsByClassName("ace_scrollbar");
        
        for (var i = 0; i < _elements.length; i++)
        {
            _elements[i].style.overflow = "hidden";
            Ps.update(_elements[i]);
        }
    }
    */

    var Content = {

        macrosArray : [],
        
        current : -1
    };

    editor.getSession().on('change', function() {
        
        if (Content.current == -1 || window.isDisable)
            return;

        Content.macrosArray[Content.current].value = editor.getValue();        

    });

    function updateMenu()
    {
        if (Content.current < 0)
            Content.current = 0;
        if (Content.current >= Content.macrosArray.length)
            Content.current = Content.macrosArray.length - 1;
            
        var menuContent = "";
        for (var i = 0; i < Content.macrosArray.length; i++)
        {
            var cl = (i == Content.current) ? "macrosSelected" : "macros";
            var item = "<div class=\"" + cl + "\" id=\"item" + i + "\" onclick=\"window.onItemClick(" + i + ");\">" + Content.macrosArray[i].name + "</div>";
            menuContent += item;
        }
        
        var elem = document.getElementById("menu_content");
        elem.innerHTML = menuContent;
        
        onItemClick(Content.current, true);
        updateScrollMenu();
    }

    function onItemClick(index, isAttack)
    {
        if (index != Content.current || isAttack)
        {
            for (var i = 0; i < Content.macrosArray.length; i++)
            {
                var elem = document.getElementById("item" + i);
                if (i == index)
                {
                    elem.classList.remove("macros");
                    elem.classList.add("macrosSelected");
                }
                else if (i == Content.current)
                {
                    elem.classList.remove("macrosSelected");
                    elem.classList.add("macros");
                }
            }
            Content.current = index;
        }
        
        if (-1 == Content.current)
        {
            window.isDisable = true;
            editor.setValue('');
            editor.setReadOnly(true);
            window.isDisable = false;
        }
        else
        {
            window.isDisable = true;
            editor.setValue(Content.macrosArray[index].value);
            editor.setReadOnly(false);
            window.isDisable = false;
        }
            
        editor.selection.clearSelection();
    }
    window.onItemClick = onItemClick;

    document.getElementById("button_new").onclick = function() {
        var indexMax = 0;
        for (var i = 0; i < Content.macrosArray.length; i++)
        {
            if (0 == Content.macrosArray[i].name.indexOf("Macros"))
            {
                var index = parseInt(Content.macrosArray[i].name.substr(6));
                if (!isNaN(index) && (indexMax < index))
                    indexMax = index;
            }
        }
        indexMax++;
        Content.macrosArray.push({ name : ("Macros " + indexMax), value : "(function()\n{\n})();" });
        Content.current = Content.macrosArray.length - 1;
        updateMenu();
    };
    document.getElementById("button_delete").onclick = function() {
        if (Content.current != -1)
        {
            Content.macrosArray.splice(Content.current, 1);
            updateMenu();
        }
    };
    document.getElementById("button_rename").onclick = function() {
        showRename();
    };
    document.getElementById("button_run").onclick = function() {
        if (Content.current != -1)
        {
            window.Asc.plugin.info.recalculate = true;
            window.Asc.plugin.executeCommand("command", Content.macrosArray[Content.current].value);
        }
    };

    document.getElementById("rename_ok").onclick = function() {
        unShowRename(true);
    };
    document.getElementById("rename_cancel").onclick = function() {
        unShowRename(false);
    };

    Ps.initialize(document.getElementById("menu"), { theme : 'custom-theme' });
    updateScrollMenu();

    function updateScrollMenu()
    {
        Ps.update(document.getElementById("menu"));
        if($('.ps__scrollbar-y').height() === 0)
        {
            $('.ps__scrollbar-y').css('border-width', '0px');
        }
        else
        {
            $('.ps__scrollbar-y').css('border-width', '1px');
        }

        if($('.ps__scrollbar-x').width() === 0)
        {
            $('.ps__scrollbar-x').css('border-width', '0px');
        }
        else
        {
            $('.ps__scrollbar-x').css('border-width', '1px');
        }
    }

    var isShowRename = false;
    function showRename()
    {
        if (Content.current < 0)
            return;

        var _elem1 = document.getElementById("idRenameMask");
        var _elem2 = document.getElementById("idRename");
        _elem1.style.display = "block";
        _elem2.style.display = "block";
        document.getElementById("rename_text").value = Content.macrosArray[Content.current].name;

        isShowRename = true;
    }

    function unShowRename(isOK)
    {
        var _elem1 = document.getElementById("idRenameMask");
        var _elem2 = document.getElementById("idRename");
        _elem1.style.display = "none";
        _elem2.style.display = "none";

        isShowRename = false;

        if (Content.current < 0)
            return;

        if (isOK)
        {
            Content.macrosArray[Content.current].name = document.getElementById("rename_text").value;
            updateMenu();
        }

        document.getElementById("rename_text").value = "";
    }

    window.onresize = function()
    {
        updateScrollMenu();
    }

    window.onkeydown = function(e)
    {
        if (isShowRename)
        {
            switch (e.keyCode)
            {
                case 27:
                {
                    unShowRename(false);
                    break;
                }
                case 13:
                {
                    unShowRename(true);
                    break;
                }
                default:
                    break;
            }
        }
    }

    window.Asc.plugin.init = function(text)
	{
        this.executeMethod("GetMacros", [JSON.stringify(Content)], function(data) {
            
            try
            {
                Content = JSON.parse(data);
            }
            catch (err)
            {
                Content = {
                    macrosArray : [],                    
                    current : -1
                };
            }
            
            updateMenu();
        });
	};
	
	window.Asc.plugin.button = function(id)
	{   
        if (id == 0)
        {
            this.executeMethod("SetMacros", [JSON.stringify(Content)], function(){
                this.executeCommand("close", "");    
            });
        }
        else
        {
            this.executeCommand("close", "");
        }
    };
    
    window.Asc.plugin.onExternalMouseUp = function()
    {
        var evt = document.createEvent("MouseEvents");
        evt.initMouseEvent("mouseup", true, true, window, 1, 0, 0, 0, 0,
            false, false, false, false, 0, null);

        document.dispatchEvent(evt);
    };

})(window, undefined);