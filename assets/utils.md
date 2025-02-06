<!--
@TableOfContents
<script run-once>
    let md = 'LIASCRIPT: \n';
    let elements = document.getElementsByClassName('lia-toc__link');
    
    let depth = parseInt("@0");
    if( isNaN(depth) )
        depth = 2;
    
    for (let element of elements) 
    {
        for (let i = 1; i <= depth; i++)
            if (element.classList.contains('lia-toc__link--is-lvl-' + i))
                md += '  '.repeat(i-1) + '- [' + element.innerHTML + '](' + element.href + ')\n\n';
    }

    md
</script>
@end
-->

# Utilities