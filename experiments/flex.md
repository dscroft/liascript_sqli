<!--
link:  ../assets/styles.css
-->

# Layout

Demonstration of a flex container with three flex children.

<section class="flex-container">

<!-- class="flex-child" style="min-width: 250px;" -->
<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d2054.5688088133725!2d13.327554047971027!3d50.92552588678725!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x47a7600bf79f1c37%3A0x40b394cc1d4e837e!2sBernhard-von-Cotta-Stra%C3%9Fe%2022%2C%2009599%20Freiberg!5e0!3m2!1sen!2sde!4v1723798197162!5m2!1sen!2sde" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>


<!-- class="flex-child" style="min-width: 250px" -->
Lorem ipsum odor amet, consectetuer adipiscing elit. Velit faucibus ut blandit commodo venenatis odio blandit. Dis fusce nunc pretium ad etiam, eleifend litora blandit. Odio lacus varius, augue donec bibendum tincidunt. Risus cras finibus posuere lobortis curae dapibus; volutpat integer. Inceptos nec donec lacinia vulputate potenti porta. Lacinia eros class facilisi adipiscing mi platea venenatis habitant aptent. Euismod molestie ut, ullamcorper vel metus neque. Placerat leo aptent natoque at gravida vel sociosqu turpis.


<!-- class="flex-child" style="min-width: 250px" -->
Lorem ipsum odor amet, consectetuer adipiscing elit. Posuere bibendum tellus natoque viverra maximus maximus tellus. Mollis ornare efficitur euismod felis rhoncus imperdiet eleifend. Turpis velit commodo arcu dictum mollis imperdiet suspendisse est maximus. Senectus pharetra porttitor torquent consectetur primis natoque litora natoque. Hac vitae facilisis porttitor porttitor nulla. Ridiculus felis hac quisque posuere hendrerit nec pulvinar himenaeos.

</section>

## How?

Put the whole thing in a in a `section` tag with a class of `flex-container`.

Each child is a `div` with a class of `flex-child`. 
Minimum width of each child can be set with the `style` attribute.

E.g.

```html
<section class="flex-container">

<!-- class="flex-child" style="min-width: 250px;" -->
Lorem ipsum odor amet.

<!-- class="flex-child" style="min-width: 250px" -->
Velit faucibus ut blandit.

</section>
```

Need the following CSS as well, recommend adding it to a seperate file and linking it in the header e.g.:

```markdown
<!--
link:  ../assets/styles.css
-->
```

```css
.flex-container {
    display: flex;
    flex-wrap: wrap; /* Allows the items to wrap as needed */
    align-items: stretch;
    gap: 20px; /* Adds both horizontal and vertical spacing between items */
}

.flex-child { 
    flex: 1;
    margin-right: 20px; /* Adds space between the columns */
}

@media (max-width: 600px) {
    .flex-child {
        flex: 100%; /* Makes the child divs take up the full width on slim devices */
        margin-right: 0; /* Removes the right margin */
    }
}
@end
```