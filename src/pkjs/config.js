module.exports = [
  {
    "type": "heading",
    "defaultValue": "WUMS Cool Watchface Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
      },
      {
        "type": "color",
        "messageKey": "LineColor",
        "allowGray": "True",
        "sunlight": false,
        "defaultValue": "0xFFFFFF",
        "label": "Line Color"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Formats"
      },
      {
         "type": "select",
          "messageKey": "DateFormat",
          "label": "Date Format",
          "defaultValue": "1",
          "options": [
            { 
              "label": "Day.Month.Year", 
              "value": "3" 
            },
            { 
              "label": "Month.Day.Year", 
              "value": "2" 
            },
            { 
              "label": "Year.Month.Date", 
              "value": "1" 
            }
          ]
      }      
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  },
  {
    "type": "text",
    "defaultValue": "Click here to donate to my beer fund :-) Thanks! <br><form action='https://www.paypal.com/cgi-bin/webscr' method='post' target='_top'> <input type='hidden' name='cmd' value='_donations'> <input type='hidden' name='business' value='wumswatchface@gmail.com'> <input type='hidden' name='lc' value='NL'> <input type='hidden' name='item_name' value='Wums Cool Watchface'> <input type='hidden' name='item_number' value='Watchface donation'> <input type='hidden' name='amount' value='2.00'> <input type='hidden' name='currency_code' value='EUR'> <input type='hidden' name='bn' value='PP-DonationsBF:btn_donate_SM.gif:NonHosted'> <input type='image' src='https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif' border='0' name='submit' alt='PayPal - The safer, easier way to pay online!'> <img alt='' border='0' src='https://www.paypalobjects.com/nl_NL/i/scr/pixel.gif' width='1' height='1'> </form>"
  }
];