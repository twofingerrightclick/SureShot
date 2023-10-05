class RemoteControls {
  isArmed: KnockoutObservable<boolean> = ko.observable(false);
  isConnected: KnockoutObservable<boolean> = ko.observable(false);

  serverAddress: KnockoutObservable<string> = ko.observable("10.42.0.144");

  constructor() {
    setInterval(this.getStatus, 300);
  }

  getStatus = async () => {
    try {
      const rawResponse = await fetch(`http://${this.serverAddress()}/status`, {
        method: "GET",
      });

      const text = await rawResponse.text();
      const data = JSON.parse(text);
      this.isArmed(!!data.armed);
      this.isConnected(true);
      
    } catch(e) {
      console.error(e);
      this.isConnected(false);
    }
  };

  fire = async () => {
    const rawResponse = await fetch(`http://${this.serverAddress()}/fire`, {
      method: "GET",
    });
  };

  toggleArmStatus = async () => {
    const url = `http://${this.serverAddress()}/${
      this.isArmed() ? "disarm" : "arm"
    }`;

    const rawResponse = await fetch(url, {
      method: "GET",
    });
  };
}

const applyViewModel = () => ko.applyBindings(new RemoteControls());

// apply only once knockout is loaded
const waitToApplyViewModel = () => {
  try {
    if (ko != undefined) {
      try {
        applyViewModel();
      } catch (error) {
        console.error(error);
      }
    }
  } catch {
    setTimeout(waitToApplyViewModel, 200);
  }
};

waitToApplyViewModel();
